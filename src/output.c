#include "common_object.h"


void add_output(output *f, output *g, int size){
  int i;
  for(i=0;i<size;i++){
#ifdef LB_FLUID
     f[i].x += g[i].x;
     f[i].y += g[i].y;
     f[i].z += g[i].z; 
     f[i].ux += g[i].ux;
     f[i].uy += g[i].uy;
     f[i].uz += g[i].uz;
     f[i].ux2 += g[i].ux2;
     f[i].uy2 += g[i].uy2;
     f[i].uz2 += g[i].uz2;
     f[i].rho += g[i].rho;
     f[i].ene += g[i].ene;
     f[i].eps += g[i].eps;
#endif
#ifdef LB_TEMPERATURE
    f[i].dxt += g[i].dxt;
    f[i].dyt += g[i].dyt;
    f[i].dzt += g[i].dzt;
    f[i].uxt += g[i].uxt;
    f[i].uyt += g[i].uyt;
    f[i].uzt += g[i].uzt;
    f[i].nux += g[i].nux;
    f[i].nuy += g[i].nuy; 
    f[i].nuz += g[i].nuz;
    f[i].t   += g[i].t;
    f[i].t2  += g[i].t2;
    f[i].epst += g[i].epst; 
    f[i].lb  += g[i].lb;
#ifdef LB_TEMPERATURE_MELTING
    f[i].lf  += g[i].lf;
    f[i].dtlf += g[i].dtlf;
    f[i].enth += g[i].enth;
#endif
#endif
  }
}



void dump_averages(){
  int i,j,k;
  FILE *fout;
  char fname[128];
  my_double x,y,z,ux,uy,uz,ux2,uy2,uz2,ene,rho,eps;
  my_double norm;
  tensor grad_u;
  int irun;
  my_double vol,lx,ly,lz,inv_lx,inv_ly,inv_lz;
#ifdef LB_TEMPERATURE
  my_double temp,t2,epst,dxt,dyt,dzt,uxt,uyt,uzt,nux,nuy,nuz,lb;
  vector grad_t;
#ifdef LB_TEMPERATURE_MELTING   
  my_double lf, dtlf, enth;
#endif 
#endif


  x=y=z=ux=uy=uz=ux2=uy2=uz2=ene=rho=eps=0.0;

#ifdef LB_TEMPERATURE
temp = t2 = epst = dxt = dyt = dzt = uxt= uyt = uzt = nux = nuy = nuz= lb = 0.0;
#endif

/* if on diagnostic */
if(itime%((int)(property.time_dump_diagn/property.time_dt))==0){

  irun = (int)( itime/ (int)(property.time_dump_diagn/property.time_dt) );

  /* set to zero local rulers */
 set_to_zero_output(ruler_x_local,NX);
 set_to_zero_output(ruler_y_local,NY);
 set_to_zero_output(ruler_z_local,NZ);

 /* set to zero global ruler */
 set_to_zero_output(ruler_x,NX);
 set_to_zero_output(ruler_y,NY);
 set_to_zero_output(ruler_z,NZ);

#ifdef LB_FLUID
    out_local.x = out_local.y = out_local.z = 0.0; 
    out_local.ux = out_local.uy = out_local.uz = 0.0; 
    out_local.ux2 = out_local.uy2 = out_local.uz2 = 0.0; 
    out_local.rho = 0.0;
    out_local.ene = 0.0;
    out_local.eps = 0.0;
    /*
    for (i = 0; i < NX; i++){ 
      ruler_x_local[i].x   = ruler_x_local[i].y   = ruler_x_local[i].z  = 0.0;
      ruler_x_local[i].ux  = ruler_x_local[i].uy  = ruler_x_local[i].uz  = 0.0;
      ruler_x_local[i].ux2 = ruler_x_local[i].uy2 = ruler_x_local[i].uz2 = 0.0;
      ruler_x_local[i].rho = ruler_x_local[i].ene = ruler_x_local[i].eps = 0.0;

      ruler_x[i].x   = ruler_x[i].y   = ruler_x[i].z  = 0.0;
      ruler_x[i].ux  = ruler_x[i].uy  = ruler_x[i].uz  = 0.0;
      ruler_x[i].ux2 = ruler_x[i].uy2 = ruler_x[i].uz2 = 0.0;
      ruler_x[i].rho = ruler_x[i].ene = ruler_x[i].eps = 0.0;
    }
    for (i = 0; i < NY; i++){ 
      ruler_y_local[i].x   = ruler_y_local[i].y   = ruler_y_local[i].z  = 0.0;
      ruler_y_local[i].ux  = ruler_y_local[i].uy  = ruler_y_local[i].uz  = 0.0;
      ruler_y_local[i].ux2 = ruler_y_local[i].uy2 = ruler_y_local[i].uz2 = 0.0;
      ruler_y_local[i].rho = ruler_y_local[i].ene = ruler_y_local[i].eps = 0.0;

      ruler_y[i].x   = ruler_y[i].y   = ruler_y[i].z  = 0.0;
      ruler_y[i].ux  = ruler_y[i].uy  = ruler_y[i].uz  = 0.0;
      ruler_y[i].ux2 = ruler_y[i].uy2 = ruler_y[i].uz2 = 0.0;
      ruler_y[i].rho = ruler_y[i].ene = ruler_y[i].eps = 0.0;
    } 
    for (i = 0; i < NZ; i++){ 
      ruler_z_local[i].x   = ruler_z_local[i].y   = ruler_z_local[i].z  = 0.0;
      ruler_z_local[i].ux  = ruler_z_local[i].uy  = ruler_z_local[i].uz  = 0.0;
      ruler_z_local[i].ux2 = ruler_z_local[i].uy2 = ruler_z_local[i].uz2 = 0.0;
      ruler_z_local[i].rho = ruler_z_local[i].ene = ruler_z_local[i].eps = 0.0;

      ruler_z[i].x   = ruler_z[i].y   = ruler_z[i].z  = 0.0;
      ruler_z[i].ux  = ruler_z[i].uy  = ruler_z[i].uz  = 0.0;
      ruler_z[i].ux2 = ruler_z[i].uy2 = ruler_z[i].uz2 = 0.0;
      ruler_z[i].rho = ruler_z[i].ene = ruler_z[i].eps = 0.0;
    }
    */
#endif


#ifdef LB_TEMPERATURE
    out_local.dxt = out_local.dyt = out_local.dzt = 0.0; 
    out_local.uxt = out_local.uyt = out_local.uzt = 0.0; 
    out_local.nux = out_local.nuy = out_local.nuz = 0.0; 
    out_local.t = 0.0;
    out_local.t2 = 0.0;
    out_local.epst = 0.0;
    out_local.lb = 0.0;
    /*
    for (i = 0; i < NX; i++){ 
      ruler_x_local[i].dxt   = ruler_x_local[i].dyt   = ruler_x_local[i].dzt  = 0.0;
      ruler_x_local[i].uxt  = ruler_x_local[i].uyt  = ruler_x_local[i].uzt  = 0.0;
      ruler_x_local[i].nux = ruler_x_local[i].nuy = ruler_x_local[i].nuz = 0.0;
      ruler_x_local[i].t = ruler_x_local[i].t2 = ruler_x_local[i].epst = 0.0;
      ruler_x_local[i].lb = 0.0;

      ruler_x[i].dxt   = ruler_x[i].dyt   = ruler_x[i].dzt  = 0.0;
      ruler_x[i].uxt  = ruler_x[i].uyt  = ruler_x[i].uzt  = 0.0;
      ruler_x[i].nux = ruler_x[i].nuy = ruler_x[i].nuz = 0.0;
      ruler_x[i].t = ruler_x[i].t2 = ruler_x[i].epst = 0.0;
      ruler_x[i].lb = 0.0;
    }
    for (i = 0; i < NY; i++){ 
      ruler_y_local[i].dxt   = ruler_y_local[i].dyt   = ruler_y_local[i].dzt  = 0.0;
      ruler_y_local[i].uxt  = ruler_y_local[i].uyt  = ruler_y_local[i].uzt  = 0.0;
      ruler_y_local[i].nux = ruler_y_local[i].nuy = ruler_y_local[i].nuz = 0.0;
      ruler_y_local[i].t = ruler_y_local[i].t2 = ruler_y_local[i].epst = 0.0;
      ruler_y_local[i].lb = 0.0;

      ruler_y[i].dxt   = ruler_y[i].dyt   = ruler_y[i].dzt  = 0.0;
      ruler_y[i].uxt  = ruler_y[i].uyt  = ruler_y[i].uzt  = 0.0;
      ruler_y[i].nux = ruler_y[i].nuy = ruler_y[i].nuz = 0.0;
      ruler_y[i].t = ruler_y[i].t2 = ruler_y[i].epst = 0.0;
      ruler_y[i].lb = 0.0;
    } 
    for (i = 0; i < NZ; i++){ 
      ruler_z_local[i].dxt   = ruler_z_local[i].dyt   = ruler_z_local[i].dzt  = 0.0;
      ruler_z_local[i].uxt  = ruler_z_local[i].uyt  = ruler_z_local[i].uzt  = 0.0;
      ruler_z_local[i].nux = ruler_z_local[i].nuy = ruler_z_local[i].nuz = 0.0;
      ruler_z_local[i].t = ruler_z_local[i].t2 = ruler_z_local[i].epst = 0.0;
      ruler_z_local[i].lb = 0.0;

      ruler_z[i].dxt   = ruler_z[i].dyt   = ruler_z[i].dzt  = 0.0;
      ruler_z[i].uxt  = ruler_z[i].uyt  = ruler_z[i].uzt  = 0.0;
      ruler_z[i].nux = ruler_z[i].nuy = ruler_z[i].nuz = 0.0;
      ruler_z[i].t = ruler_z[i].t2 = ruler_z[i].epst = 0.0;
      ruler_z[i].lb = 0.0;
    }
    */
#ifdef LB_TEMPERATURE_MELTING
    out_local.lf = out_local.dtlf = out_local.enth = 0.0; 
    /*
    for (i = 0; i < NX; i++){ 
      ruler_x_local[i].lf   = ruler_x_local[i].dtlf   = ruler_x_local[i].enth  = 0.0;
      ruler_x[i].lf   = ruler_x[i].dtlf   = ruler_x[i].enth  = 0.0;
    }

    for (i = 0; i < NY; i++){ 
      ruler_y_local[i].lf   = ruler_y_local[i].dtlf   = ruler_y_local[i].enth  = 0.0;
      ruler_y[i].lf   = ruler_y[i].dtlf   = ruler_y[i].enth  = 0.0;
    } 

    for (i = 0; i < NZ; i++){ 
      ruler_z_local[i].lf   = ruler_z_local[i].dtlf   = ruler_z_local[i].enth  = 0.0;
      ruler_z[i].lf   = ruler_z[i].dtlf   = ruler_z[i].enth  = 0.0;
    }
    */
#endif
#endif


    /* Here we send recv to compute the gradients */
#ifdef LB_FLUID
    sendrecv_borders_vector(u);
#endif
#ifdef LB_TEMPERATURE
    sendrecv_borders_scalar(t);
#endif



	for (i = BRD; i < LNX+BRD; i++)
		for (j = BRD; j < LNY+BRD; j++)
			for (k = BRD; k < LNZ+BRD; k++) {


			  /* computing volume local */
			  lx = 0.5*(center_V[IDX(i+1, j, k)].x - center_V[IDX(i-1, j, k)].x);
			  ly = 0.5*(center_V[IDX(i, j+1, k)].y - center_V[IDX(i, j-1, k)].y);
			  lz = 0.5*(center_V[IDX(i, j, k+1)].z - center_V[IDX(i, j, k-1)].z);
			  vol = lx*ly*lz;
			  inv_lx = 1./lx;
			  inv_ly = 1./ly;
			  inv_lz = 1./lz;
#ifdef DEBUG			 
			  fprintf(stderr,"%d %d %d vol %e lx %e,ly %e, lz %e\n",i,j,k,vol,lx,ly,lz);
			  //vol=1.0;
#endif

#ifdef LB_FLUID
			  
			  out_local.x += x = center_V[IDX(i, j, k)].x;
			  ruler_x_local[i -BRD + LNX_START].x += x;
			  ruler_y_local[j -BRD + LNY_START].x += x;
			  ruler_z_local[k -BRD + LNZ_START].x += x;

			  out_local.y += y = center_V[IDX(i, j, k)].y;
			  ruler_x_local[i -BRD + LNX_START].y += y;
			  ruler_y_local[j -BRD + LNY_START].y += y;
			  ruler_z_local[k -BRD + LNZ_START].y += y;

			  out_local.z += z = center_V[IDX(i, j, k)].z;
			  ruler_x_local[i -BRD + LNX_START].z += z;
			  ruler_y_local[j -BRD + LNY_START].z += z;
			  ruler_z_local[k -BRD + LNZ_START].z += z;
			  
			  out_local.ux += ux = u[IDX(i, j, k)].x*vol;
			  ruler_x_local[i -BRD + LNX_START].ux += ux*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].ux += ux*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].ux += ux*inv_lz;

			  out_local.uy += uy = u[IDX(i, j, k)].y*vol;
			  ruler_x_local[i -BRD + LNX_START].uy += uy*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].uy += uy*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].uy += uy*inv_lz;

			  out_local.uz += uz = u[IDX(i, j, k)].z*vol;
			  ruler_x_local[i -BRD + LNX_START].uz += uz*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].uz += uz*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].uz += uz*inv_lz;

			  ux2 = u[IDX(i, j, k)].x*u[IDX(i, j, k)].x*vol;
			  out_local.ux2 += ux2;
			  ruler_x_local[i -BRD + LNX_START].ux2 += ux2*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].ux2 += ux2*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].ux2 += ux2*inv_lz;

			  uy2 = u[IDX(i, j, k)].y*u[IDX(i, j, k)].y*vol;
			  out_local.uy2 += uy2;
			  ruler_x_local[i -BRD + LNX_START].uy2 += uy2*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].uy2 += uy2*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].uy2 += uy2*inv_lz;

			  uz2 = u[IDX(i, j, k)].z*u[IDX(i, j, k)].z*vol;
			  out_local.uz2 += uz2;
			  ruler_x_local[i -BRD + LNX_START].uz2 += uz2*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].uz2 += uz2*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].uz2 += uz2*inv_lz;

			  out_local.rho += rho = dens[IDX(i, j, k)]*vol;
			  ruler_x_local[i -BRD + LNX_START].rho += rho*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].rho += rho*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].rho += rho*inv_lz;

	    out_local.ene += ene = 0.5*(u[IDX(i, j, k)].x*u[IDX(i, j, k)].x + u[IDX(i, j, k)].y*u[IDX(i, j, k)].y + u[IDX(i, j, k)].z*u[IDX(i, j, k)].z)*vol; // 0.5*(ux2+uy2+uz2);
			  ruler_x_local[i -BRD + LNX_START].ene += ene*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].ene += ene*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].ene += ene*inv_lz;
			  
			  //S=strain_tensor(p,i, j, k);
			  grad_u = gradient_vector(u,i,j,k);
			  // fprintf(stderr,"SXY %e\n",S.xy);
			                   eps = ( (grad_u.xx + grad_u.xx)*(grad_u.xx + grad_u.xx) + 
			                           (grad_u.xy + grad_u.yx)*(grad_u.xy + grad_u.yx) +
			                           (grad_u.xz + grad_u.zx)*(grad_u.xz + grad_u.zx) +
			                           (grad_u.yx + grad_u.xy)*(grad_u.yx + grad_u.xy) + 
			                           (grad_u.yy + grad_u.yy)*(grad_u.yy + grad_u.yy) +
			                           (grad_u.yz + grad_u.zy)*(grad_u.yz + grad_u.zy) +
                                                   (grad_u.zx + grad_u.xz)*(grad_u.zx + grad_u.xz) + 
			                           (grad_u.zy + grad_u.yz)*(grad_u.zy + grad_u.yz) +
			                           (grad_u.zz + grad_u.zz)*(grad_u.zz + grad_u.zz) ) *0.5 * property.nu * vol;
			  out_local.eps += eps;			  
			  ruler_x_local[i -BRD + LNX_START].eps += eps*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].eps += eps*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].eps += eps*inv_lz;
#endif


#ifdef LB_TEMPERATURE
			  /* NB: this gradient definition assumes a (refined or not) cartesian grid */
			  /* note thet t field must be communicate first -> it has been done above */
			  grad_t=gradient_scalar(t,i,j,k);

			  dxt = grad_t.x*vol;
			  out_local.dxt += dxt;
			  ruler_x_local[i -BRD + LNX_START].dxt += dxt*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].dxt += dxt*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].dxt += dxt*inv_lz;

			  dyt = grad_t.y*vol;
			  out_local.dyt += dyt;
			  ruler_x_local[i -BRD + LNX_START].dyt += dyt*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].dyt += dyt*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].dyt += dyt*inv_lz;

			  dzt = grad_t.z*vol;
			  out_local.dzt += dzt;
			  ruler_x_local[i -BRD + LNX_START].dzt += dzt*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].dzt += dzt*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].dzt += dzt*inv_lz;
			  

			  out_local.uxt += uxt = u[IDX(i, j, k)].x*t[IDX(i, j, k)]*vol;
			  ruler_x_local[i -BRD + LNX_START].uxt += uxt*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].uxt += uxt*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].uxt += uxt*inv_lz;

			  out_local.uyt += uyt = u[IDX(i, j, k)].y*t[IDX(i, j, k)]*vol;
			  ruler_x_local[i -BRD + LNX_START].uyt += uyt*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].uyt += uyt*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].uyt += uyt*inv_lz;

			  out_local.uzt += uzt = u[IDX(i, j, k)].z*t[IDX(i, j, k)]*vol;
			  ruler_x_local[i -BRD + LNX_START].uzt += uzt*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].uzt += uzt*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].uzt += uzt*inv_lz;

			  nux = ( - property.kappa*dxt + uxt );
			  out_local.nux += nux;
			  ruler_x_local[i -BRD + LNX_START].nux += nux*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].nux += nux*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].nux += nux*inv_lz;

			  nuy = ( - property.kappa*dyt + uyt );
			  out_local.nuy += nuy;
			  ruler_x_local[i -BRD + LNX_START].nuy += nuy*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].nuy += nuy*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].nuy += nuy*inv_lz;

			  nuz = ( - property.kappa*dzt + uzt );
			  out_local.nuz += nuz;
			  ruler_x_local[i -BRD + LNX_START].nuz += nuz*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].nuz += nuz*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].nuz += nuz*inv_lz;

			  out_local.t += temp = t[IDX(i, j, k)]*vol;
			  ruler_x_local[i -BRD + LNX_START].t += temp*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].t += temp*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].t += temp*inv_lz;

			  t2=t[IDX(i, j, k)]*t[IDX(i, j, k)]*vol;
			  out_local.t2 += t2;
			  ruler_x_local[i -BRD + LNX_START].t2 += t2*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].t2 += t2*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].t2 += t2*inv_lz;
			  
			  epst = property.kappa*(grad_t.x*grad_t.x + grad_t.y*grad_t.y + grad_t.z*grad_t.z)*vol; //property.kappa*(dxt*dxt + dyt*dyt + dzt*dzt);
			  out_local.epst += epst; 
			  ruler_x_local[i -BRD + LNX_START].epst += epst*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].epst += epst*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].epst += epst*inv_lz;
#ifdef LB_TEMPERATURE_BUOYANCY
			  lb = 0; //pow(eps,5./4.)*pow(epst,-3./4.)*pow(property.gravity_y*property.beta_t,-3./2.);
			  out_local.lb += lb;
			  ruler_x_local[i -BRD + LNX_START].lb += lb;
			  ruler_y_local[j -BRD + LNY_START].lb += lb;
			  ruler_z_local[k -BRD + LNZ_START].lb += lb;
#endif

#ifdef LB_TEMPERATURE_MELTING   
			  lf = liquid_frac[IDX(i, j, k)]*vol;
			  out_local.lf += lf;
			  ruler_x_local[i -BRD + LNX_START].lf += lf*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].lf += lf*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].lf += lf*inv_lz;

			  dtlf = ( liquid_frac[IDX(i, j, k)]-liquid_frac_old[IDX(i, j, k)] )*(vol/property.time_dt);
			  out_local.dtlf += dtlf;
			  ruler_x_local[i -BRD + LNX_START].dtlf += dtlf*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].dtlf += dtlf*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].dtlf += dtlf*inv_lz;

			  enth = ( property.specific_heat*t[IDX(i,j,k)] + property.latent_heat*liquid_frac[IDX(i,j,k)])*vol;
			  out_local.enth += enth;
			  ruler_x_local[i -BRD + LNX_START].enth += enth*inv_lx;
			  ruler_y_local[j -BRD + LNY_START].enth += enth*inv_ly;
			  ruler_z_local[k -BRD + LNZ_START].enth += enth*inv_lz;
#endif 

#endif

			} /* for i j k */      




	/* Sum all */
  MPI_Allreduce(&out_local, &out_all, 1, MPI_output_type, MPI_SUM_output, MPI_COMM_WORLD );

  MPI_Allreduce(ruler_x_local, ruler_x, NX, MPI_output_type, MPI_SUM_output, MPI_COMM_WORLD );
  MPI_Allreduce(ruler_y_local, ruler_y, NY, MPI_output_type, MPI_SUM_output, MPI_COMM_WORLD );
  MPI_Allreduce(ruler_z_local, ruler_z, NZ, MPI_output_type, MPI_SUM_output, MPI_COMM_WORLD );

#ifdef LB_FLUID

#define OUTPUT_NORM
#ifdef OUTPUT_NORM
  /* normalization */
  //norm = 1.0/(my_double)(NX*NY*NZ);
  norm = 1.0/(my_double)(property.SX*property.SY*property.SZ);
  out_all.ene *= norm;
  out_all.rho *= norm;
  out_all.ux  *= norm;
  out_all.uy  *= norm;
  out_all.uz  *= norm;
  out_all.ux2 *= norm;
  out_all.uy2 *= norm;
  out_all.uz2 *= norm;
  out_all.eps *= norm;


  //norm = 1.0/(my_double)(NY*NZ);
    norm = 1.0/(my_double)(property.SY*property.SZ);
  for (i = 0; i < NX; i++){
    ruler_x[i].x *= 1.0/(my_double)(NY*NZ); //norm;
    ruler_x[i].ene *= norm;
    ruler_x[i].rho *= norm;
    ruler_x[i].ux *= norm; 
    ruler_x[i].uy *= norm;
    ruler_x[i].uz *= norm;
    ruler_x[i].ux2 *= norm;
    ruler_x[i].uy2 *= norm;
    ruler_x[i].uz2 *= norm;
    ruler_x[i].eps *= norm;
  }

  //norm = 1.0/(my_double)(NX*NZ);
  norm = 1.0/(my_double)(property.SX*property.SZ);
  for (i = 0; i < NY; i++){
    ruler_y[i].y *= 1.0/(my_double)(NX*NZ); //norm;
    ruler_y[i].ene *= norm;
    ruler_y[i].rho *= norm;
    ruler_y[i].ux *= norm; 
    ruler_y[i].uy *= norm;
    ruler_y[i].uz *= norm;
    ruler_y[i].ux2 *= norm;
    ruler_y[i].uy2 *= norm;
    ruler_y[i].uz2 *= norm;
    ruler_y[i].eps *= norm;
  }

  //norm = 1.0/(my_double)(NX*NY);
  norm = 1.0/(my_double)(property.SX*property.SY);
  for (i = 0; i < NZ; i++){
    ruler_z[i].z *= 1.0/(my_double)(NX*NY); //norm;
    ruler_z[i].ene *= norm;
    ruler_z[i].rho *= norm;
    ruler_z[i].ux *= norm; 
    ruler_z[i].uy *= norm;
    ruler_z[i].uz *= norm;
    ruler_z[i].ux2 *= norm;
    ruler_z[i].uy2 *= norm;
    ruler_z[i].uz2 *= norm;
    ruler_z[i].eps *= norm;
  }
#endif
#endif 

#ifdef LB_TEMPERATURE

#ifdef OUTPUT_NORM
  /* normalization */
  //norm = 1.0/(my_double)(NX*NY*NZ);
  norm = 1.0/(my_double)(property.SX*property.SY*property.SZ);
  out_all.t *= norm;
  out_all.t2 *= norm;
  out_all.epst  *= norm;
  out_all.dxt  *= norm;
  out_all.dyt  *= norm;
  out_all.dzt *= norm;
  out_all.uxt *= norm;
  out_all.uyt *= norm;
  out_all.uzt *= norm;
  out_all.nux *= norm/( property.kappa*property.deltaT/property.SY );
  out_all.nuy *= norm/( property.kappa*property.deltaT/property.SY );
  out_all.nuz *= norm/( property.kappa*property.deltaT/property.SY );
#ifdef LB_TEMPERATURE_BUOYANCY
  out_all.lb  = pow(out_all.eps,5./4.)*pow(out_all.epst,-3./4.)*pow(property.gravity_y*property.beta_t,-3./2.);
#else
  out_all.lb  = 0.0; 
#endif

  //norm = 1.0/(my_double)(NY*NZ);
  norm = 1.0/(my_double)(property.SY*property.SZ);
  for (i = 0; i < NX; i++){
    ruler_x[i].t *= norm;
    ruler_x[i].t2 *= norm;
    ruler_x[i].epst *= norm;
    ruler_x[i].dxt *= norm; 
    ruler_x[i].dyt *= norm;
    ruler_x[i].dzt *= norm;
    ruler_x[i].uxt *= norm;
    ruler_x[i].uyt *= norm;
    ruler_x[i].uzt *= norm;
    ruler_x[i].nux *= norm/( property.kappa*property.deltaT/property.SY );
    ruler_x[i].nuy *= norm/( property.kappa*property.deltaT/property.SY );
    ruler_x[i].nuz *= norm/( property.kappa*property.deltaT/property.SY );
#ifdef LB_TEMPERATURE_BUOYANCY
    ruler_x[i].lb = pow(ruler_x[i].eps,5./4.)*pow(ruler_x[i].epst,-3./4.)*pow(property.gravity_y*property.beta_t,-3./2.);
#else
    ruler_x[i].lb = 0.0;
#endif
  }

  //norm = 1.0/(my_double)(NX*NZ);
  norm = 1.0/(my_double)(property.SX*property.SZ);
  for (i = 0; i < NY; i++){
    ruler_y[i].t *= norm;
    ruler_y[i].t2 *= norm;
    ruler_y[i].epst *= norm;
    ruler_y[i].dxt *= norm; 
    ruler_y[i].dyt *= norm;
    ruler_y[i].dzt *= norm;
    ruler_y[i].uxt *= norm;
    ruler_y[i].uyt *= norm;
    ruler_y[i].uzt *= norm;
    ruler_y[i].nux *= norm/( property.kappa*property.deltaT/property.SY );
    ruler_y[i].nuy *= norm/( property.kappa*property.deltaT/property.SY );
    ruler_y[i].nuz *= norm/( property.kappa*property.deltaT/property.SY );
#ifdef LB_TEMPERATURE_BUOYANCY
    ruler_y[i].lb =  pow(ruler_y[i].eps,5./4.)*pow(ruler_y[i].epst,-3./4.)*pow(property.gravity_y*property.beta_t,-3./2.);
#else
    ruler_y[i].lb = 0.0;
#endif
  }

  //norm = 1.0/(my_double)(NX*NY);
  norm = 1.0/(my_double)(property.SY*property.SZ);
  for (i = 0; i < NZ; i++){
    ruler_z[i].t *= norm;
    ruler_z[i].t2 *= norm;
    ruler_z[i].epst *= norm;
    ruler_z[i].dxt *= norm; 
    ruler_z[i].dyt *= norm;
    ruler_z[i].dzt *= norm;
    ruler_z[i].uxt *= norm;
    ruler_z[i].uyt *= norm;
    ruler_z[i].uzt *= norm;
    ruler_z[i].nux *= norm/( property.kappa*property.deltaT/property.SY );
    ruler_z[i].nuy *= norm/( property.kappa*property.deltaT/property.SY );
    ruler_z[i].nuz *= norm/( property.kappa*property.deltaT/property.SY );
#ifdef LB_TEMPERATURE_BUOYANCY
    ruler_z[i].lb =  pow(ruler_z[i].eps,5./4.)*pow(ruler_z[i].epst,-3./4.)*pow(property.gravity_y*property.beta_t,-3./2.);
#else
    ruler_z[i].lb = 0.0;
#endif

  }
#endif

#ifdef LB_TEMPERATURE_MELTING
  /* normalization */
  // norm = 1.0/(my_double)(NX*NY*NZ);
  norm = 1.0/(my_double)(property.SX*property.SY*property.SZ);
  out_all.lf *= norm;
  out_all.dtlf *= norm;
  out_all.enth  *= norm;

  //norm = 1.0/(my_double)(NY*NZ);
  norm = 1.0/(my_double)(property.SY*property.SZ);
  for (i = 0; i < NX; i++){
    ruler_x[i].lf *= norm;
    ruler_x[i].dtlf *= norm;
    ruler_x[i].enth *= norm;
  }

  //norm = 1.0/(my_double)(NX*NZ);
  norm = 1.0/(my_double)(property.SX*property.SZ);
  for (i = 0; i < NY; i++){
    ruler_y[i].lf *= norm;
    ruler_y[i].dtlf *= norm;
    ruler_y[i].enth *= norm;
  }

  //norm = 1.0/(my_double)(NX*NY);
  norm = 1.0/(my_double)(property.SX*property.SY);
  for (i = 0; i < NZ; i++){
    ruler_z[i].lf *= norm;
    ruler_z[i].dtlf *= norm;
    ruler_z[i].enth *= norm;
  }
#endif
#endif 


  /* Here running averages */
  add_output(ruler_x_running,ruler_x,NX);
  add_output(ruler_y_running,ruler_y,NY);
  add_output(ruler_z_running,ruler_z,NZ);


#ifdef LB_FLUID
  if(ROOT){
    sprintf(fname,"velocity_averages.dat");
    fout = fopen(fname,"a");    
    fprintf(fout,"%e %e %e %e %e %e %e %e %e %e\n",time_now, (double)out_all.ene,(double)out_all.rho, (double)out_all.ux, (double)out_all.uy, (double)out_all.uz, (double)out_all.ux2 , (double)out_all.uy2, (double)out_all.uz2, (double)out_all.eps);
    fclose(fout);

    sprintf(fname,"velocity_averages_x.dat");
    fout = fopen(fname,"w");
    for (i = 0; i < NX; i++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e\n",(double)ruler_x[i].x, (double)ruler_x[i].ene, (double)ruler_x[i].rho, (double)ruler_x[i].ux, (double)ruler_x[i].uy, (double)ruler_x[i].uz, (double)ruler_x[i].ux2 , (double)ruler_x[i].uy2, (double)ruler_x[i].uz2, (double)ruler_x[i].eps);
    fclose(fout);

    sprintf(fname,"velocity_averages_y.dat");
    fout = fopen(fname,"w");
    for (j = 0; j < NY; j++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e\n",(double)ruler_y[j].y, (double)ruler_y[j].ene, (double)ruler_y[j].rho, (double)ruler_y[j].ux, (double)ruler_y[j].uy, (double)ruler_y[j].uz, (double)ruler_y[j].ux2 , (double)ruler_y[j].uy2, (double)ruler_y[j].uz2,(double)ruler_y[j].eps);
    fclose(fout);

    sprintf(fname,"velocity_averages_z.dat");
    fout = fopen(fname,"w");
    for (k = 0; k < NZ; k++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e\n",(double)ruler_z[k].z, (double)ruler_z[k].ene, (double)ruler_z[k].rho, (double)ruler_z[k].ux, (double)ruler_z[k].uy, (double)ruler_z[k].uz, (double)ruler_z[k].ux2 , (double)ruler_z[k].uy2, (double)ruler_z[k].uz2, (double)ruler_z[k].eps);
    fclose(fout);

    sprintf(fname,"velocity_averages_x_run.dat");
    fout = fopen(fname,"w");
    for (i = 0; i < NX; i++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e\n",(double)ruler_x_running[i].x/(double)irun, (double)ruler_x_running[i].ene/(double)irun, (double)ruler_x_running[i].rho/(double)irun, (double)ruler_x_running[i].ux/(double)irun, (double)ruler_x_running[i].uy/(double)irun, (double)ruler_x_running[i].uz/(double)irun, (double)ruler_x_running[i].ux2/(double)irun , (double)ruler_x_running[i].uy2/(double)irun, (double)ruler_x_running[i].uz2/(double)irun, (double)ruler_x_running[i].eps/(double)irun);
    fclose(fout);

    sprintf(fname,"velocity_averages_y_run.dat");
    fout = fopen(fname,"w");
    for (j = 0; j < NY; j++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e\n",(double)ruler_y_running[j].y/(double)irun, (double)ruler_y_running[j].ene/(double)irun, (double)ruler_y_running[j].rho/(double)irun, (double)ruler_y_running[j].ux/(double)irun, (double)ruler_y_running[j].uy/(double)irun, (double)ruler_y_running[j].uz/(double)irun, (double)ruler_y_running[j].ux2/(double)irun , (double)ruler_y_running[j].uy2/(double)irun, (double)ruler_y_running[j].uz2/(double)irun,(double)ruler_y_running[i].eps/(double)irun);
    fclose(fout);

    sprintf(fname,"velocity_averages_z_run.dat");
    fout = fopen(fname,"w");
    for (k = 0; k < NZ; k++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e\n",(double)ruler_z_running[k].z/(double)irun, (double)ruler_z_running[k].ene/(double)irun, (double)ruler_z_running[k].rho/(double)irun, (double)ruler_z_running[k].ux/(double)irun, (double)ruler_z_running[k].uy/(double)irun, (double)ruler_z_running[k].uz/(double)irun, (double)ruler_z_running[k].ux2/(double)irun , (double)ruler_z_running[k].uy2/(double)irun, (double)ruler_z_running[k].uz2/(double)irun, (double)ruler_z_running[i].eps/(double)irun);
    fclose(fout);


  }
#endif
  
#ifdef LB_TEMPERATURE
  if(ROOT){
    sprintf(fname,"temperature_averages.dat");
    fout = fopen(fname,"a");    
    fprintf(fout,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",time_now, (double)out_all.t,(double)out_all.t2, (double)out_all.epst, (double)out_all.dxt, (double)out_all.dyt, (double)out_all.dzt , (double)out_all.uxt, (double)out_all.uyt, (double)out_all.uzt,(double)out_all.nux, (double)out_all.nuy, (double)out_all.nuz,(double)out_all.lb );
    fclose(fout);

    sprintf(fname,"temperature_averages_x.dat");
    fout = fopen(fname,"w");
    for (i = 0; i < NX; i++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",(double)ruler_x[i].x, (double)ruler_x[i].t, (double)ruler_x[i].t2, (double)ruler_x[i].epst, (double)ruler_x[i].dxt, (double)ruler_x[i].dyt, (double)ruler_x[i].dzt , (double)ruler_x[i].uxt, (double)ruler_x[i].uyt, (double)ruler_x[i].uzt, (double)ruler_x[i].nux, (double)ruler_x[i].nuy, (double)ruler_x[i].nuz, (double)ruler_x[i].lb);
    fclose(fout);

    sprintf(fname,"temperature_averages_y.dat");
    fout = fopen(fname,"w");
    for (j = 0; j < NY; j++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",(double)ruler_y[j].y, (double)ruler_y[j].t, (double)ruler_y[j].t2, (double)ruler_y[j].epst, (double)ruler_y[j].dxt, (double)ruler_y[j].dyt, (double)ruler_y[j].dzt , (double)ruler_y[j].uxt, (double)ruler_y[j].uyt,(double)ruler_y[i].uzt, (double)ruler_y[j].nux, (double)ruler_y[j].nuy, (double)ruler_y[j].nuz,(double)ruler_y[j].lb);
    fclose(fout);

    sprintf(fname,"temperature_averages_z.dat");
    fout = fopen(fname,"w");
    for (k = 0; k < NZ; k++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",(double)ruler_z[k].z, (double)ruler_z[k].t, (double)ruler_z[k].t2, (double)ruler_z[k].epst, (double)ruler_z[k].dxt, (double)ruler_z[k].dyt, (double)ruler_z[k].dzt , (double)ruler_z[k].uxt, (double)ruler_z[k].uyt, (double)ruler_z[i].uzt, (double)ruler_z[k].nux, (double)ruler_z[k].nuy, (double)ruler_z[k].nuz,(double)ruler_z[k].lb );
    fclose(fout);


    sprintf(fname,"temperature_averages_x_run.dat");
    fout = fopen(fname,"w");
    for (i = 0; i < NX; i++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",(double)ruler_x_running[i].x/(double)irun, (double)ruler_x_running[i].t/(double)irun, (double)ruler_x_running[i].t2/(double)irun, (double)ruler_x_running[i].epst/(double)irun, (double)ruler_x_running[i].dxt/(double)irun, (double)ruler_x_running[i].dyt/(double)irun, (double)ruler_x_running[i].dzt/(double)irun , (double)ruler_x_running[i].uxt/(double)irun, (double)ruler_x_running[i].uyt/(double)irun, (double)ruler_x_running[i].uzt/(double)irun, (double)ruler_x_running[i].nux/(double)irun, (double)ruler_x_running[i].nuy/(double)irun, (double)ruler_x_running[i].nuz/(double)irun, (double)ruler_x_running[i].lb/(double)irun);
    fclose(fout);

    sprintf(fname,"temperature_averages_y_run.dat");
    fout = fopen(fname,"w");
    for (j = 0; j < NY; j++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",(double)ruler_y_running[j].y/(double)irun, (double)ruler_y_running[j].t/(double)irun, (double)ruler_y_running[j].t2/(double)irun, (double)ruler_y_running[j].epst/(double)irun, (double)ruler_y_running[j].dxt/(double)irun, (double)ruler_y_running[j].dyt/(double)irun, (double)ruler_y_running[j].dzt/(double)irun , (double)ruler_y_running[j].uxt/(double)irun, (double)ruler_y_running[j].uyt/(double)irun,(double)ruler_y_running[i].uzt/(double)irun, (double)ruler_y_running[j].nux/(double)irun, (double)ruler_y_running[j].nuy/(double)irun, (double)ruler_y_running[j].nuz/(double)irun,(double)ruler_y_running[j].lb/(double)irun);
    fclose(fout);

    sprintf(fname,"temperature_averages_z_run.dat");
    fout = fopen(fname,"w");
    for (k = 0; k < NZ; k++) fprintf(fout,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",(double)ruler_z_running[k].z/(double)irun, (double)ruler_z_running[k].t/(double)irun, (double)ruler_z_running[k].t2/(double)irun, (double)ruler_z_running[k].epst/(double)irun, (double)ruler_z_running[k].dxt/(double)irun, (double)ruler_z_running[k].dyt/(double)irun, (double)ruler_z_running[k].dzt/(double)irun , (double)ruler_z_running[k].uxt/(double)irun, (double)ruler_z_running[k].uyt/(double)irun, (double)ruler_z_running[i].uzt/(double)irun, (double)ruler_z_running[k].nux/(double)irun, (double)ruler_z_running[k].nuy/(double)irun, (double)ruler_z_running[k].nuz/(double)irun,(double)ruler_z_running[k].lb/(double)irun );
    fclose(fout);
  }

#ifdef LB_TEMPERATURE_MELTING
  if(ROOT){
    /* global */
    sprintf(fname,"melting_averages.dat");
    fout = fopen(fname,"a");    
    fprintf(fout,"%e %e %e %e %e\n",time_now, (double)out_all.lf,(double)out_all.dtlf, (double)out_all.enth);
    fclose(fout);

    /* instantaneous */
    sprintf(fname,"melting_averages_x.dat");
    fout = fopen(fname,"w");
    for (i = 0; i < NX; i++) fprintf(fout,"%e %e %e %e %e\n",(double)ruler_x[i].x, (double)ruler_x[i].lf, (double)ruler_x[i].dtlf, (double)ruler_x[i].enth);
    fclose(fout);

    sprintf(fname,"melting_averages_y.dat");
    fout = fopen(fname,"w");
    for (j = 0; j < NY; j++) fprintf(fout,"%e %e %e %e %e\n",(double)ruler_y[j].y, (double)ruler_y[j].lf, (double)ruler_y[j].dtlf, (double)ruler_y[j].enth);
    fclose(fout);

    sprintf(fname,"melting_averages_z.dat");
    fout = fopen(fname,"w");
    for (k = 0; k < NZ; k++) fprintf(fout,"%e %e %e %e %e\n",(double)ruler_z[k].z, (double)ruler_z[k].lf, (double)ruler_z[k].dtlf, (double)ruler_z[k].enth);
    fclose(fout);

    /* running  averages */
    sprintf(fname,"melting_averages_x_run.dat");
    fout = fopen(fname,"w");
    for (i = 0; i < NX; i++) fprintf(fout,"%e %e %e %e %e\n",(double)ruler_x_running[i].x/(double)irun, (double)ruler_x_running[i].lf/(double)irun, (double)ruler_x_running[i].dtlf/(double)irun, (double)ruler_x_running[i].enth/(double)irun);
    fclose(fout);

    sprintf(fname,"melting_averages_y_run.dat");
    fout = fopen(fname,"w");
    for (j = 0; j < NY; j++) fprintf(fout,"%e %e %e %e %e\n",(double)ruler_y_running[j].y/(double)irun, (double)ruler_y_running[j].lf/(double)irun, (double)ruler_y_running[j].dtlf/(double)irun, (double)ruler_y_running[j].enth/(double)irun);
    fclose(fout);

    sprintf(fname,"melting_averages_z_run.dat");
    fout = fopen(fname,"w");
    for (k = 0; k < NZ; k++) fprintf(fout,"%e %e %e %e %e\n",(double)ruler_z_running[k].z/(double)irun, (double)ruler_z_running[k].lf/(double)irun, (double)ruler_z_running[k].dtlf/(double)irun, (double)ruler_z_running[k].enth/(double)irun);
    fclose(fout);
  }
#endif
#endif


}/* end of if on time_dump_diagn */

#ifdef OUTPUT_H5
    if(itime%((int)(property.time_dump_field/property.time_dt))==0) output_h5();
#endif


#ifdef OUTPUT_ASCII
#ifdef LB_FLUID

  if(ROOT && itime%((int)(property.time_dump_field/property.time_dt))==0 ){
  /* Here dumps the velocity field */
    sprintf(fname,"%s/vel.%d",OutDir,itime);
  fout = fopen(fname,"w");

  for(k=BRD;k<LNZ+BRD;k++){
    for(j=BRD;j<LNY+BRD;j++){
      for(i=BRD;i<LNX+BRD;i++){ 

        fprintf(fout,"%e %e %e %e %e %e %e\n", 
		(double)center_V[IDX(i, j, k)].x, (double)center_V[IDX(i, j, k)].y, (double)center_V[IDX(i, j, k)].z, 
                (double)u[IDX(i,j,k)].x, (double)u[IDX(i,j,k)].y, (double)u[IDX(i,j,k)].z,
		(double)dens[IDX(i,j,k)] );  
      } 
      fprintf(fout,"\n");
    }
    fprintf(fout,"\n");
  }
    fclose(fout);
  }
#endif

#ifdef LB_TEMPERATURE
  if(ROOT  && itime%((int)(property.time_dump_field/property.time_dt))==0){
  /* Here dumps the temperature field */
    sprintf(fname,"%s/temp.%d",OutDir,itime);
  fout = fopen(fname,"w");

  for(k=BRD;k<LNZ+BRD;k++){
    for(j=BRD;j<LNY+BRD;j++){
      for(i=BRD;i<LNX+BRD;i++){ 

        fprintf(fout,"%e %e %e %e\n", 
		(double)center_V[IDX(i, j, k)].x, (double)center_V[IDX(i, j, k)].y, (double)center_V[IDX(i, j, k)].z, 
		(double)t[IDX(i,j,k)] );  
      } 
      fprintf(fout,"\n");
    }
    fprintf(fout,"\n");
  }
    fclose(fout);
  }
#endif

#endif
}

