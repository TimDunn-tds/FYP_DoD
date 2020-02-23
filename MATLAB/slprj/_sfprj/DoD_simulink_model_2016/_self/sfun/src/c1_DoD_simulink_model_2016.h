#ifndef __c1_DoD_simulink_model_2016_h__
#define __c1_DoD_simulink_model_2016_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_c1_struct_T
#define typedef_c1_struct_T

typedef struct {
  int32_T f0;
  int32_T f1;
} c1_struct_T;

#endif                                 /*typedef_c1_struct_T*/

#include <stdlib.h>
#ifndef struct_tag_scZlGNcmSdXsfgNftx2nQBF
#define struct_tag_scZlGNcmSdXsfgNftx2nQBF

struct tag_scZlGNcmSdXsfgNftx2nQBF
{
  char_T DataType[6];
  real_T MaxIter;
  real_T FeasibilityTol;
  boolean_T IntegrityChecks;
};

#endif                                 /*struct_tag_scZlGNcmSdXsfgNftx2nQBF*/

#ifndef typedef_c1_scZlGNcmSdXsfgNftx2nQBF
#define typedef_c1_scZlGNcmSdXsfgNftx2nQBF

typedef struct tag_scZlGNcmSdXsfgNftx2nQBF c1_scZlGNcmSdXsfgNftx2nQBF;

#endif                                 /*typedef_c1_scZlGNcmSdXsfgNftx2nQBF*/

#ifndef typedef_c1_cell_0
#define typedef_c1_cell_0

typedef struct {
  real_T f1[800];
} c1_cell_0;

#endif                                 /*typedef_c1_cell_0*/

#ifndef typedef_SFc1_DoD_simulink_model_2016InstanceStruct
#define typedef_SFc1_DoD_simulink_model_2016InstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c1_sfEvent;
  boolean_T c1_doneDoubleBufferReInit;
  uint8_T c1_is_active_c1_DoD_simulink_model_2016;
  real_T c1_Linv[400];
  c1_scZlGNcmSdXsfgNftx2nQBF c1_opt;
  real_T c1_n_in;
  real_T c1_Gamma[100];
  real_T c1_Acd[9];
  real_T c1_Bcd[3];
  real_T c1_Aineq[800];
  real_T c1_bineq[40];
  real_T *c1_ref;
  real_T *c1_u;
  real_T (*c1_mu0)[3];
  real_T *c1_u0;
  real_T *c1_status;
} SFc1_DoD_simulink_model_2016InstanceStruct;

#endif                                 /*typedef_SFc1_DoD_simulink_model_2016InstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray
  *sf_c1_DoD_simulink_model_2016_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c1_DoD_simulink_model_2016_get_check_sum(mxArray *plhs[]);
extern void c1_DoD_simulink_model_2016_method_dispatcher(SimStruct *S, int_T
  method, void *data);

#endif
