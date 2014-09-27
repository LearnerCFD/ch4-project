#include "common_object.h"

#ifdef LB_FLUID_LES
my_double tau_u_les(int i , int j , int k){

  vector grad_u;
  my_double s_norm, nu_les, tau; 
  my_double C_smag = 0.2;

  
  grad_u = gradient_vector(u,i,j,k);

  /* Norm of S */
  s_norm = sqrt( 0.5*( (grad_u.xx + grad_u.xx)*(grad_u.xx + grad_u.xx) + 
		    (grad_u.xy + grad_u.yx)*(grad_u.xy + grad_u.yx) +
		    (grad_u.xz + grad_u.zx)*(grad_u.xz + grad_u.zx) +
		    (grad_u.yx + grad_u.xy)*(grad_u.yx + grad_u.xy) + 
		    (grad_u.yy + grad_u.yy)*(grad_u.yy + grad_u.yy) +
		    (grad_u.yz + grad_u.zy)*(grad_u.yz + grad_u.zy) +
		    (grad_u.zx + grad_u.xz)*(grad_u.zx + grad_u.xz) + 
		    (grad_u.zy + grad_u.yz)*(grad_u.zy + grad_u.yz) +
		    (grad_u.zz + grad_u.zz)*(grad_u.zz + grad_u.zz) );


#ifdef METHOD_STREAMING
		          vol = 1.0;
#else  		 
                          /* computing volume local */
                          lx = 0.5*(center_V[IDX(i+1, j, k)].x - center_V[IDX(i-1, j, k)].x);
                          ly = 0.5*(center_V[IDX(i, j+1, k)].y - center_V[IDX(i, j-1, k)].y);
                          lz = 0.5*(center_V[IDX(i, j, k+1)].z - center_V[IDX(i, j, k-1)].z);
                          vol = lx*ly*lz;
#endif

		 /* eddy viscosity */
		 nu_les = s_norm * pow( C_smag * vol, 2.0 ); 		 
		  

#ifdef METHOD_STREAMING
  tau_les = 3.0*nu_les + 0.5;		 
#else
#ifdef METHOD_REDEFINED_POP
  tau_les = 3.0*nu_les + 0.5*property.time_dt;
#else
  tau_les = 3.0*nu_les;
#endif
#endif

return tau_les;

}
#endif


#ifdef LB_TEMPERATURE_LES
my_double tau_t_les(int i , int j , int k){

    my_double tau_temp;
    my_double prandtl = property.nu/property.kappa;

    tau_les = tau_u_les(int i , int j , int k);

#ifdef METHOD_STREAMING
    tau_temp = (tau_les+property.tau_u-0.5)/prandtl + 0.5 -property.tau_t;
#else
#ifdef METHOD_REDEFINED_POP
    tau_temp = (tau_les+property.tau_u-0.5*property.time_dt)/prandtl + 0.5*property.time_dt -property.tau_t;
#else
    tau_temp = (tau_les+property.tau_u)/prandtl -property.tau_t;
#endif
#endif

    return tau_temp;
  }
#endif

#ifdef LB_SCALAR_LES
my_double tau_s_les(int i , int j , int k){

    my_double tau_scal;
    my_double schmidt = property.nu/property.chi;

    tau_les = tau_u_les(int i , int j , int k);

#ifdef METHOD_STREAMING
    tau_scal = (tau_les+property.tau_u-0.5)/schmidt + 0.5 -property.tau_s;
#else
#ifdef METHOD_REDEFINED_POP
    tau_scal = (tau_les+property.tau_u-0.5*property.time_dt)/schmidt + 0.5*property.time_dt -property.tau_s;
#else
    tau_scal = (tau_les+property.tau_u)/schmidt -property.tau_s;
#endif
#endif

    return tau_scal;
  }
#endif





  }
#endif
