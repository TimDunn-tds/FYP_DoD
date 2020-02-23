/* Include files */

#include "DoD_simulink_model_2016_sfun.h"
#include "c1_DoD_simulink_model_2016.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "DoD_simulink_model_2016_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c_with_debugger(S, sfGlobalDebugInstanceStruct);

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization);
static void chart_debug_initialize_data_addresses(SimStruct *S);
static const mxArray* sf_opaque_get_hover_data_for_msg(void *chartInstance,
  int32_T msgSSID);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)
#define c1_const_n_in                  (1.0)
#define c1_b_n_in                      (1.0)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c1_debug_family_names[20] = { "mu1", "kk", "g", "iA0",
  "uMPC", "Linv", "opt", "n_in", "nargin", "nargout", "ref", "mu0", "u0",
  "Gamma", "Acd", "Bcd", "Aineq", "bineq", "u", "status" };

/* Function Declarations */
static void initialize_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void initialize_params_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void enable_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void disable_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void c1_update_debugger_state_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static const mxArray *get_sim_state_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void set_sim_state_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, const mxArray
   *c1_st);
static void finalize_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void sf_gateway_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void mdl_start_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void initSimStructsc1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber, uint32_T c1_instanceNumber);
static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData);
static real_T c1_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_status, const char_T *c1_identifier);
static real_T c1_b_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_c_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[40]);
static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_d_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[800]);
static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_e_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[3]);
static void c1_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_e_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_f_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[9]);
static void c1_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_f_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_g_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[100]);
static void c1_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_g_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static const mxArray *c1_h_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static const mxArray *c1_i_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_h_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[20]);
static void c1_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_j_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static const mxArray *c1_k_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_i_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[5]);
static void c1_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static void c1_mpcqpsolver(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_f[20], real_T c1_A[800], real_T c1_b_b[40], real_T
  c1_x[20], real_T *c1_b_status);
static void c1_check_forloop_overflow_error
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, boolean_T
   c1_overflow);
static void c1_qpkwik(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                      real_T c1_Hinv[400], real_T c1_f[20], real_T c1_Ac[800],
                      real_T c1_b_b[40], int16_T c1_maxiter, real_T c1_FeasTol,
                      real_T c1_x[20], real_T c1_lambda[40], real_T *c1_b_status,
                      int16_T c1_iA[40]);
static void c1_KWIKfactor(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Ac[800], int16_T c1_iC[40], int16_T c1_nA, real_T
  c1_RLinv[400], real_T c1_D[400], real_T c1_H[400], real_T c1_b_RLinv[400],
  real_T c1_b_D[400], real_T c1_b_H[400], real_T *c1_Status);
static void c1_b_check_forloop_overflow_error
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, boolean_T
   c1_overflow);
static void c1_qr(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                  real_T c1_A[400], real_T c1_Q[400], real_T c1_R[400]);
static void c1_xgeqrf(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                      real_T c1_A[400], real_T c1_b_A[400], real_T c1_tau[20]);
static void c1_error(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance);
static void c1_b_error(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
  int32_T c1_varargin_2);
static void c1_c_error(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
  int32_T c1_varargin_2);
static void c1_DropConstraint(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, int16_T c1_kDrop, int16_T c1_iA[40], int16_T c1_nA, int16_T
  c1_iC[40], int16_T c1_b_iA[40], int16_T *c1_b_nA, int16_T c1_b_iC[40]);
static void c1_Unconstrained(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Hinv[400], real_T c1_f[20], real_T c1_x[20], int16_T
  c1_n, real_T c1_b_x[20]);
static real_T c1_norm(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                      real_T c1_x[20]);
static void c1_abs(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                   real_T c1_x[20], real_T c1_y[20]);
static void c1_b_abs(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                     real_T c1_x[40], real_T c1_y[40]);
static const mxArray *c1_l_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static int32_T c1_j_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static void c1_k_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[400]);
static void c1_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static c1_scZlGNcmSdXsfgNftx2nQBF c1_l_emlrt_marshallIn
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, const mxArray
   *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_m_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  char_T c1_y[6]);
static boolean_T c1_n_emlrt_marshallIn
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, const mxArray
   *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static uint8_T c1_o_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance, const mxArray *c1_b_is_active_c1_DoD_simulink_model_2016, const
  char_T *c1_identifier);
static uint8_T c1_p_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId);
static real_T c1_b_KWIKfactor(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Ac[800], int16_T c1_iC[40], int16_T c1_nA, real_T
  c1_RLinv[400], real_T c1_D[400], real_T c1_H[400]);
static void c1_b_xgeqrf(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_A[400], real_T c1_tau[20]);
static void c1_b_DropConstraint(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, int16_T c1_kDrop, int16_T c1_iA[40], int16_T *c1_nA, int16_T
  c1_iC[40]);
static void c1_b_Unconstrained(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Hinv[400], real_T c1_f[20], real_T c1_x[20], int16_T
  c1_n);
static void init_dsm_address_info(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance);
static void init_simulink_io_address(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance);

/* Function Definitions */
static void initialize_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  if (sf_is_first_init_cond(chartInstance->S)) {
    initSimStructsc1_DoD_simulink_model_2016(chartInstance);
    chart_debug_initialize_data_addresses(chartInstance->S);
  }

  chartInstance->c1_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c1_is_active_c1_DoD_simulink_model_2016 = 0U;
}

static void initialize_params_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  real_T c1_dv0[400];
  int32_T c1_i0;
  const mxArray *c1_m0 = NULL;
  static const char * c1_fieldNames[4] = { "DataType", "MaxIter",
    "FeasibilityTol", "IntegrityChecks" };

  const mxArray *c1_mxField;
  c1_scZlGNcmSdXsfgNftx2nQBF c1_r0;
  real_T c1_d0;
  real_T c1_dv1[100];
  int32_T c1_i1;
  real_T c1_dv2[9];
  int32_T c1_i2;
  real_T c1_dv3[3];
  int32_T c1_i3;
  real_T c1_dv4[800];
  int32_T c1_i4;
  real_T c1_dv5[40];
  int32_T c1_i5;
  sf_mex_import_named("Linv", sf_mex_get_sfun_param(chartInstance->S, 4, 0),
                      c1_dv0, 0, 0, 0U, 1, 0U, 2, 20, 20);
  for (c1_i0 = 0; c1_i0 < 400; c1_i0++) {
    chartInstance->c1_Linv[c1_i0] = c1_dv0[c1_i0];
  }

  c1_m0 = sf_mex_get_sfun_param(chartInstance->S, 7, 1);
  sf_mex_check_bus_parameter(c1_m0, 4, c1_fieldNames, "scZlGNcmSdXsfgNftx2nQBF");
  c1_mxField = sf_mex_getfield(c1_m0, "DataType", "opt", 0);
  sf_mex_import_named("opt", sf_mex_dup(c1_mxField), c1_r0.DataType, 1, 10, 0U,
                      1, 0U, 2, 1, 6);
  c1_mxField = sf_mex_getfield(c1_m0, "MaxIter", "opt", 0);
  sf_mex_import_named("opt", sf_mex_dup(c1_mxField), &c1_r0.MaxIter, 1, 0, 0U, 0,
                      0U, 0);
  c1_mxField = sf_mex_getfield(c1_m0, "FeasibilityTol", "opt", 0);
  sf_mex_import_named("opt", sf_mex_dup(c1_mxField), &c1_r0.FeasibilityTol, 1, 0,
                      0U, 0, 0U, 0);
  c1_mxField = sf_mex_getfield(c1_m0, "IntegrityChecks", "opt", 0);
  sf_mex_import_named("opt", sf_mex_dup(c1_mxField), &c1_r0.IntegrityChecks, 1,
                      11, 0U, 0, 0U, 0);
  sf_mex_destroy(&c1_m0);
  chartInstance->c1_opt = c1_r0;
  sf_mex_import_named("n_in", sf_mex_get_sfun_param(chartInstance->S, 6, 0),
                      &c1_d0, 0, 0, 0U, 0, 0U, 0);
  chartInstance->c1_n_in = c1_d0;
  sf_mex_import_named("Gamma", sf_mex_get_sfun_param(chartInstance->S, 3, 0),
                      c1_dv1, 0, 0, 0U, 1, 0U, 2, 20, 5);
  for (c1_i1 = 0; c1_i1 < 100; c1_i1++) {
    chartInstance->c1_Gamma[c1_i1] = c1_dv1[c1_i1];
  }

  sf_mex_import_named("Acd", sf_mex_get_sfun_param(chartInstance->S, 0, 0),
                      c1_dv2, 0, 0, 0U, 1, 0U, 2, 3, 3);
  for (c1_i2 = 0; c1_i2 < 9; c1_i2++) {
    chartInstance->c1_Acd[c1_i2] = c1_dv2[c1_i2];
  }

  sf_mex_import_named("Bcd", sf_mex_get_sfun_param(chartInstance->S, 2, 0),
                      c1_dv3, 0, 0, 0U, 1, 0U, 1, 3);
  for (c1_i3 = 0; c1_i3 < 3; c1_i3++) {
    chartInstance->c1_Bcd[c1_i3] = c1_dv3[c1_i3];
  }

  sf_mex_import_named("Aineq", sf_mex_get_sfun_param(chartInstance->S, 1, 0),
                      c1_dv4, 0, 0, 0U, 1, 0U, 2, 40, 20);
  for (c1_i4 = 0; c1_i4 < 800; c1_i4++) {
    chartInstance->c1_Aineq[c1_i4] = c1_dv4[c1_i4];
  }

  sf_mex_import_named("bineq", sf_mex_get_sfun_param(chartInstance->S, 5, 0),
                      c1_dv5, 0, 0, 0U, 1, 0U, 1, 40);
  for (c1_i5 = 0; c1_i5 < 40; c1_i5++) {
    chartInstance->c1_bineq[c1_i5] = c1_dv5[c1_i5];
  }
}

static void enable_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c1_update_debugger_state_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  const mxArray *c1_st;
  const mxArray *c1_y = NULL;
  real_T c1_hoistedGlobal;
  const mxArray *c1_b_y = NULL;
  real_T c1_b_hoistedGlobal;
  const mxArray *c1_c_y = NULL;
  uint8_T c1_c_hoistedGlobal;
  const mxArray *c1_d_y = NULL;
  c1_st = NULL;
  c1_st = NULL;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_createcellmatrix(3, 1), false);
  c1_hoistedGlobal = *chartInstance->c1_status;
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", &c1_hoistedGlobal, 0, 0U, 0U, 0U, 0),
                false);
  sf_mex_setcell(c1_y, 0, c1_b_y);
  c1_b_hoistedGlobal = *chartInstance->c1_u;
  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", &c1_b_hoistedGlobal, 0, 0U, 0U, 0U,
    0), false);
  sf_mex_setcell(c1_y, 1, c1_c_y);
  c1_c_hoistedGlobal = chartInstance->c1_is_active_c1_DoD_simulink_model_2016;
  c1_d_y = NULL;
  sf_mex_assign(&c1_d_y, sf_mex_create("y", &c1_c_hoistedGlobal, 3, 0U, 0U, 0U,
    0), false);
  sf_mex_setcell(c1_y, 2, c1_d_y);
  sf_mex_assign(&c1_st, c1_y, false);
  return c1_st;
}

static void set_sim_state_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, const mxArray
   *c1_st)
{
  const mxArray *c1_b_u;
  chartInstance->c1_doneDoubleBufferReInit = true;
  c1_b_u = sf_mex_dup(c1_st);
  *chartInstance->c1_status = c1_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell("status", c1_b_u, 0)), "status");
  *chartInstance->c1_u = c1_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell("u", c1_b_u, 1)), "u");
  chartInstance->c1_is_active_c1_DoD_simulink_model_2016 = c1_o_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(
       "is_active_c1_DoD_simulink_model_2016", c1_b_u, 2)),
     "is_active_c1_DoD_simulink_model_2016");
  sf_mex_destroy(&c1_b_u);
  c1_update_debugger_state_c1_DoD_simulink_model_2016(chartInstance);
  sf_mex_destroy(&c1_st);
}

static void finalize_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  int32_T c1_i6;
  real_T c1_hoistedGlobal;
  real_T c1_b_hoistedGlobal;
  real_T c1_b_ref;
  int32_T c1_i7;
  real_T c1_b_u0;
  real_T c1_b_mu0[3];
  int32_T c1_i8;
  int32_T c1_i9;
  real_T c1_b_Gamma[100];
  int32_T c1_i10;
  real_T c1_b_Acd[9];
  int32_T c1_i11;
  real_T c1_b_Bcd[3];
  int32_T c1_i12;
  real_T c1_b_Aineq[800];
  uint32_T c1_debug_family_var_map[20];
  real_T c1_b_bineq[40];
  real_T c1_mu1[3];
  real_T c1_kk[5];
  real_T c1_g[20];
  boolean_T c1_iA0[40];
  real_T c1_uMPC[20];
  real_T c1_b_Linv[400];
  c1_scZlGNcmSdXsfgNftx2nQBF c1_b_opt;
  real_T c1_c_n_in;
  real_T c1_nargin = 11.0;
  real_T c1_nargout = 2.0;
  real_T c1_b_u;
  real_T c1_b_status;
  static c1_scZlGNcmSdXsfgNftx2nQBF c1_r1 = { { 'd', 'o', 'u', 'b', 'l', 'e' },/* DataType */
    200.0,                             /* MaxIter */
    1.0E-6,                            /* FeasibilityTol */
    true                               /* IntegrityChecks */
  };

  int32_T c1_i13;
  static real_T c1_dv6[400] = { 0.17953118100446194, -0.35596589217268981,
    -0.31212981554100422, -0.22675653524786851, -0.13836709348478629,
    -0.065390865873951043, -0.017153889967133736, 0.0078411135547972188,
    0.016792892042678826, 0.017113889112630693, 0.01404679602414171,
    0.010482631107179853, 0.007609144225287896, 0.0056503763174164147,
    0.0044191163595166541, 0.0036378843765188257, 0.0030806749308000141,
    0.0026078701975466852, 0.0021524874181201204, 0.0026191208278017273, 0.0,
    0.42314360962586206, -0.15085880017441863, -0.24895431198843393,
    -0.21205994187589813, -0.1357940312684259, -0.065402106435703475,
    -0.017973592955685527, 0.006754449033805573, 0.015731139145787214,
    0.016185625920936923, 0.013258331794745552, 0.0098036329352445932,
    0.0070043049750641261, 0.0050913460341440427, 0.0038871871558543722,
    0.0031226486759438109, 0.0025774372668732233, 0.0021152123431113232,
    0.0026019082051462069, 0.0, 0.0, 0.5395265851382075, -0.014262270707080226,
    -0.18109207087873616, -0.18485281862460215, -0.12923880337950336,
    -0.0688067570398097, -0.025014422905191645, -0.00047862591763447643,
    0.0096940723600378014, 0.011566364650929865, 0.0097942556978274555,
    0.0071348572763342095, 0.0048392200597921981, 0.0032452913254430115,
    0.0022647328163359392, 0.0016850269629045736, 0.0013143364410911163,
    0.0016216283611085358, 0.0, 0.0, 0.0, 0.59211926551196981,
    0.047328381414249318, -0.15268622922557878, -0.17729389107872212,
    -0.13199197751842245, -0.075511091762731422, -0.032129433380572163,
    -0.0064963725742828932, 0.0050730630574488929, 0.0081158154215212278,
    0.0071668280151789554, 0.0050399411987726694, 0.0030882386581324041,
    0.0017374311523575998, 0.00095607535964168831, 0.00056105746294453937,
    0.00059898831032237419, 0.0, 0.0, 0.0, 0.0, 0.61112340659458753,
    0.066428855805202885, -0.14633752595678243, -0.1781025731904336,
    -0.13587450839489759, -0.080056532210652731, -0.036169443736289414,
    -0.0096980143063973984, 0.0026362238008596039, 0.0062504460102224026,
    0.0056942409780984986, 0.003836387614978537, 0.0020818899277130921,
    0.0008919675437983743, 0.00025484061500147552, -4.9249698557432218E-5, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.61609645345964925, 0.070789842264748715,
    -0.14512159360004248, -0.17874273757436696, -0.13739387415204857,
    -0.081770412971494066, -0.037737852126643358, -0.011019124823261331,
    0.0015581893404897267, 0.0053808926434729936, 0.0049980320983291594,
    0.0032855725395688645, 0.001655529200958293, 0.00057437073779722082,
    -0.00015152176150694629, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.61752066298854769,
    0.073139856854092244, -0.14313191294784083, -0.17757747081585479,
    -0.13698087464975267, -0.081855393455244174, -0.038094337555445383,
    -0.011482825060102213, 0.001096673102422268, 0.0049859139481912883,
    0.004699451064870676, 0.0030899366817990831, 0.0015564764276950239,
    0.000383387430390087, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.618877712158251,
    0.076332096826689511, -0.13998345115540026, -0.1753057517130274,
    -0.13566229424971757, -0.081286692035685393, -0.038015764072695642,
    -0.011662981367827761, 0.00082945324616624837, 0.0047397989263093642,
    0.0045303506316400708, 0.0030184046344605936, 0.0015945134104339185, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.620588804519038, 0.080166901496609044,
    -0.13624480592327431, -0.17253210532067745, -0.13397358139977011,
    -0.080492196616852987, -0.037831682769763404, -0.011820631184603076,
    0.000536667336363688, 0.0044473630990359064, 0.00431418933685784,
    0.00333741516032808, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.62248419151390333, 0.084335946763323524, -0.132123897204966,
    -0.16940390222489501, -0.13203533829528402, -0.079581195629996465,
    -0.037652580568818823, -0.012075207685536573, 8.8255026121634059E-5,
    0.0039706503037129281, 0.004983569800658629, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.62451257844946806, 0.088881192432794964,
    -0.12750262621125666, -0.16581111357097594, -0.12977631038826129,
    -0.078530614410585509, -0.037505507434398136, -0.012501775660427579,
    -0.00063570557752505607, 0.0049863787430134976, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.626749742358861, 0.094031612726143993,
    -0.12211944601290357, -0.16152819643017485, -0.12703444898697747,
    -0.077255324545548953, -0.037390146457159851, -0.013192755931829855,
    0.00047065099914092822, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.629321414091852, 0.10009345772427337, -0.11562337865774583,
    -0.15623097718874451, -0.1235574443351526, -0.0756096624066169,
    -0.037301806748486946, -0.012768012332613427, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.63238080360261406, 0.10745698452253324,
    -0.10752667340175709, -0.14943043251889698, -0.11893450275079041,
    -0.073338786154699, -0.039135056992419641, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.63612782549517355, 0.11667325035042805,
    -0.097082020512002751, -0.14031711491355309, -0.11242510917588491,
    -0.079928086371240215, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.64085382460804885, 0.12859974089490123,
    -0.083036421923058462, -0.12741027099252486, -0.12674359755585715, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.64702223690479121, 0.14468716622648875, -0.063080140974317231,
    -0.15127058736482824, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6554317041768738, 0.16762711643300396,
    -0.093565438726647021, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.66759498812525553, 0.14412109784377947,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.68298139053534912 };

  int32_T c1_i14;
  int32_T c1_i15;
  real_T c1_b_a[9];
  int32_T c1_i16;
  real_T c1_b_b[3];
  int32_T c1_i17;
  real_T c1_y[3];
  int32_T c1_i18;
  int32_T c1_i19;
  real_T c1_c_b;
  int32_T c1_i20;
  int32_T c1_i21;
  int32_T c1_i22;
  int32_T c1_i23;
  int32_T c1_i24;
  real_T c1_c_a[100];
  int32_T c1_i25;
  real_T c1_d_b[5];
  int32_T c1_i26;
  int32_T c1_i27;
  int32_T c1_i28;
  int32_T c1_i29;
  int32_T c1_i30;
  int32_T c1_i31;
  real_T c1_b_g[20];
  int32_T c1_i32;
  real_T c1_c_Aineq[800];
  real_T c1_c_bineq[40];
  real_T c1_b_uMPC[20];
  real_T c1_c_status;
  int32_T c1_i33;
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c1_u0, 2U, 1U, 0U,
                        chartInstance->c1_sfEvent, false);
  for (c1_i6 = 0; c1_i6 < 3; c1_i6++) {
    _SFD_DATA_RANGE_CHECK((*chartInstance->c1_mu0)[c1_i6], 1U, 1U, 0U,
                          chartInstance->c1_sfEvent, false);
  }

  _SFD_DATA_RANGE_CHECK(*chartInstance->c1_ref, 0U, 1U, 0U,
                        chartInstance->c1_sfEvent, false);
  chartInstance->c1_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  c1_hoistedGlobal = *chartInstance->c1_ref;
  c1_b_hoistedGlobal = *chartInstance->c1_u0;
  c1_b_ref = c1_hoistedGlobal;
  for (c1_i7 = 0; c1_i7 < 3; c1_i7++) {
    c1_b_mu0[c1_i7] = (*chartInstance->c1_mu0)[c1_i7];
  }

  c1_b_u0 = c1_b_hoistedGlobal;
  for (c1_i8 = 0; c1_i8 < 100; c1_i8++) {
    c1_b_Gamma[c1_i8] = chartInstance->c1_Gamma[c1_i8];
  }

  for (c1_i9 = 0; c1_i9 < 9; c1_i9++) {
    c1_b_Acd[c1_i9] = chartInstance->c1_Acd[c1_i9];
  }

  for (c1_i10 = 0; c1_i10 < 3; c1_i10++) {
    c1_b_Bcd[c1_i10] = chartInstance->c1_Bcd[c1_i10];
  }

  for (c1_i11 = 0; c1_i11 < 800; c1_i11++) {
    c1_b_Aineq[c1_i11] = chartInstance->c1_Aineq[c1_i11];
  }

  for (c1_i12 = 0; c1_i12 < 40; c1_i12++) {
    c1_b_bineq[c1_i12] = chartInstance->c1_bineq[c1_i12];
  }

  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 20U, 20U, c1_debug_family_names,
    c1_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_mu1, 0U, c1_d_sf_marshallOut,
    c1_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_kk, 1U, c1_k_sf_marshallOut,
    c1_h_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_g, 2U, c1_i_sf_marshallOut,
    c1_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c1_iA0, 3U, c1_j_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_uMPC, 4U, c1_i_sf_marshallOut,
    c1_g_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(c1_b_Linv, 5U, c1_h_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_b_opt, 6U, c1_g_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_c_n_in, 7U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargin, 8U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_nargout, 9U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_b_ref, 10U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(c1_b_mu0, 11U, c1_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c1_b_u0, 12U, c1_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_Gamma, 13U, c1_f_sf_marshallOut,
    c1_f_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_Acd, 14U, c1_e_sf_marshallOut,
    c1_e_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_Bcd, 15U, c1_d_sf_marshallOut,
    c1_d_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_Aineq, 16U, c1_c_sf_marshallOut,
    c1_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(c1_b_bineq, 17U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_b_u, 18U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c1_b_status, 19U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  c1_c_n_in = c1_b_n_in;
  c1_b_opt = c1_r1;
  for (c1_i13 = 0; c1_i13 < 400; c1_i13++) {
    c1_b_Linv[c1_i13] = c1_dv6[c1_i13];
  }

  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 5);
  for (c1_i14 = 0; c1_i14 < 9; c1_i14++) {
    c1_b_a[c1_i14] = c1_b_Acd[c1_i14];
  }

  for (c1_i15 = 0; c1_i15 < 3; c1_i15++) {
    c1_b_b[c1_i15] = c1_b_mu0[c1_i15];
  }

  for (c1_i16 = 0; c1_i16 < 3; c1_i16++) {
    c1_y[c1_i16] = 0.0;
    c1_i18 = 0;
    for (c1_i19 = 0; c1_i19 < 3; c1_i19++) {
      c1_y[c1_i16] += c1_b_a[c1_i18 + c1_i16] * c1_b_b[c1_i19];
      c1_i18 += 3;
    }
  }

  for (c1_i17 = 0; c1_i17 < 3; c1_i17++) {
    c1_b_b[c1_i17] = c1_b_Bcd[c1_i17];
  }

  c1_c_b = c1_b_u0;
  for (c1_i20 = 0; c1_i20 < 3; c1_i20++) {
    c1_b_b[c1_i20] *= c1_c_b;
  }

  for (c1_i21 = 0; c1_i21 < 3; c1_i21++) {
    c1_mu1[c1_i21] = c1_y[c1_i21] + c1_b_b[c1_i21];
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 8);
  for (c1_i22 = 0; c1_i22 < 3; c1_i22++) {
    c1_kk[c1_i22] = c1_mu1[c1_i22];
  }

  c1_kk[3] = c1_b_u0;
  c1_kk[4] = c1_b_ref;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 9);
  for (c1_i23 = 0; c1_i23 < 100; c1_i23++) {
    c1_c_a[c1_i23] = c1_b_Gamma[c1_i23];
  }

  for (c1_i24 = 0; c1_i24 < 5; c1_i24++) {
    c1_d_b[c1_i24] = c1_kk[c1_i24];
  }

  for (c1_i25 = 0; c1_i25 < 20; c1_i25++) {
    c1_g[c1_i25] = 0.0;
  }

  for (c1_i26 = 0; c1_i26 < 20; c1_i26++) {
    c1_g[c1_i26] = 0.0;
    c1_i28 = 0;
    for (c1_i29 = 0; c1_i29 < 5; c1_i29++) {
      c1_g[c1_i26] += c1_c_a[c1_i28 + c1_i26] * c1_d_b[c1_i29];
      c1_i28 += 20;
    }
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 11);
  for (c1_i27 = 0; c1_i27 < 40; c1_i27++) {
    c1_iA0[c1_i27] = false;
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 15);
  for (c1_i30 = 0; c1_i30 < 20; c1_i30++) {
    c1_b_g[c1_i30] = c1_g[c1_i30];
  }

  for (c1_i31 = 0; c1_i31 < 800; c1_i31++) {
    c1_c_Aineq[c1_i31] = -c1_b_Aineq[c1_i31];
  }

  for (c1_i32 = 0; c1_i32 < 40; c1_i32++) {
    c1_c_bineq[c1_i32] = -c1_b_bineq[c1_i32];
  }

  c1_mpcqpsolver(chartInstance, c1_b_g, c1_c_Aineq, c1_c_bineq, c1_b_uMPC,
                 &c1_c_status);
  for (c1_i33 = 0; c1_i33 < 20; c1_i33++) {
    c1_uMPC[c1_i33] = c1_b_uMPC[c1_i33];
  }

  c1_b_status = c1_c_status;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 18);
  c1_b_u = c1_uMPC[0];
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, -18);
  _SFD_SYMBOL_SCOPE_POP();
  *chartInstance->c1_u = c1_b_u;
  *chartInstance->c1_status = c1_b_status;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_DoD_simulink_model_2016MachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c1_u, 3U, 1U, 0U,
                        chartInstance->c1_sfEvent, false);
  _SFD_DATA_RANGE_CHECK(*chartInstance->c1_status, 4U, 1U, 0U,
                        chartInstance->c1_sfEvent, false);
}

static void mdl_start_c1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void initSimStructsc1_DoD_simulink_model_2016
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber, uint32_T c1_instanceNumber)
{
  (void)c1_machineNumber;
  (void)c1_chartNumber;
  (void)c1_instanceNumber;
}

static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  real_T c1_b_u;
  const mxArray *c1_y = NULL;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_b_u = *(real_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static real_T c1_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_status, const char_T *c1_identifier)
{
  real_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_y = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_status),
    &c1_thisId);
  sf_mex_destroy(&c1_b_status);
  return c1_y;
}

static real_T c1_b_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  real_T c1_y;
  real_T c1_d1;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), &c1_d1, 1, 0, 0U, 0, 0U, 0);
  c1_y = c1_d1;
  sf_mex_destroy(&c1_b_u);
  return c1_y;
}

static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_status;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_status = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_y = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_status),
    &c1_thisId);
  sf_mex_destroy(&c1_b_status);
  *(real_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i34;
  const mxArray *c1_y = NULL;
  real_T c1_b_u[40];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  for (c1_i34 = 0; c1_i34 < 40; c1_i34++) {
    c1_b_u[c1_i34] = (*(real_T (*)[40])c1_inData)[c1_i34];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 1, 40), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static void c1_c_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[40])
{
  real_T c1_dv7[40];
  int32_T c1_i35;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv7, 1, 0, 0U, 1, 0U, 1, 40);
  for (c1_i35 = 0; c1_i35 < 40; c1_i35++) {
    c1_y[c1_i35] = c1_dv7[c1_i35];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_bineq;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[40];
  int32_T c1_i36;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_bineq = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_bineq), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_b_bineq);
  for (c1_i36 = 0; c1_i36 < 40; c1_i36++) {
    (*(real_T (*)[40])c1_outData)[c1_i36] = c1_y[c1_i36];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i37;
  int32_T c1_i38;
  const mxArray *c1_y = NULL;
  int32_T c1_i39;
  real_T c1_b_u[800];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_i37 = 0;
  for (c1_i38 = 0; c1_i38 < 20; c1_i38++) {
    for (c1_i39 = 0; c1_i39 < 40; c1_i39++) {
      c1_b_u[c1_i39 + c1_i37] = (*(real_T (*)[800])c1_inData)[c1_i39 + c1_i37];
    }

    c1_i37 += 40;
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 2, 40, 20),
                false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static void c1_d_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[800])
{
  real_T c1_dv8[800];
  int32_T c1_i40;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv8, 1, 0, 0U, 1, 0U, 2, 40,
                20);
  for (c1_i40 = 0; c1_i40 < 800; c1_i40++) {
    c1_y[c1_i40] = c1_dv8[c1_i40];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_Aineq;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[800];
  int32_T c1_i41;
  int32_T c1_i42;
  int32_T c1_i43;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_Aineq = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_Aineq), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_b_Aineq);
  c1_i41 = 0;
  for (c1_i42 = 0; c1_i42 < 20; c1_i42++) {
    for (c1_i43 = 0; c1_i43 < 40; c1_i43++) {
      (*(real_T (*)[800])c1_outData)[c1_i43 + c1_i41] = c1_y[c1_i43 + c1_i41];
    }

    c1_i41 += 40;
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i44;
  const mxArray *c1_y = NULL;
  real_T c1_b_u[3];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  for (c1_i44 = 0; c1_i44 < 3; c1_i44++) {
    c1_b_u[c1_i44] = (*(real_T (*)[3])c1_inData)[c1_i44];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 1, 3), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static void c1_e_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[3])
{
  real_T c1_dv9[3];
  int32_T c1_i45;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv9, 1, 0, 0U, 1, 0U, 1, 3);
  for (c1_i45 = 0; c1_i45 < 3; c1_i45++) {
    c1_y[c1_i45] = c1_dv9[c1_i45];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_Bcd;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[3];
  int32_T c1_i46;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_Bcd = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_e_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_Bcd), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_b_Bcd);
  for (c1_i46 = 0; c1_i46 < 3; c1_i46++) {
    (*(real_T (*)[3])c1_outData)[c1_i46] = c1_y[c1_i46];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_e_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i47;
  int32_T c1_i48;
  const mxArray *c1_y = NULL;
  int32_T c1_i49;
  real_T c1_b_u[9];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_i47 = 0;
  for (c1_i48 = 0; c1_i48 < 3; c1_i48++) {
    for (c1_i49 = 0; c1_i49 < 3; c1_i49++) {
      c1_b_u[c1_i49 + c1_i47] = (*(real_T (*)[9])c1_inData)[c1_i49 + c1_i47];
    }

    c1_i47 += 3;
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 2, 3, 3), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static void c1_f_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[9])
{
  real_T c1_dv10[9];
  int32_T c1_i50;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv10, 1, 0, 0U, 1, 0U, 2, 3,
                3);
  for (c1_i50 = 0; c1_i50 < 9; c1_i50++) {
    c1_y[c1_i50] = c1_dv10[c1_i50];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_Acd;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[9];
  int32_T c1_i51;
  int32_T c1_i52;
  int32_T c1_i53;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_Acd = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_Acd), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_b_Acd);
  c1_i51 = 0;
  for (c1_i52 = 0; c1_i52 < 3; c1_i52++) {
    for (c1_i53 = 0; c1_i53 < 3; c1_i53++) {
      (*(real_T (*)[9])c1_outData)[c1_i53 + c1_i51] = c1_y[c1_i53 + c1_i51];
    }

    c1_i51 += 3;
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_f_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i54;
  int32_T c1_i55;
  const mxArray *c1_y = NULL;
  int32_T c1_i56;
  real_T c1_b_u[100];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_i54 = 0;
  for (c1_i55 = 0; c1_i55 < 5; c1_i55++) {
    for (c1_i56 = 0; c1_i56 < 20; c1_i56++) {
      c1_b_u[c1_i56 + c1_i54] = (*(real_T (*)[100])c1_inData)[c1_i56 + c1_i54];
    }

    c1_i54 += 20;
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 2, 20, 5),
                false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static void c1_g_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[100])
{
  real_T c1_dv11[100];
  int32_T c1_i57;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv11, 1, 0, 0U, 1, 0U, 2, 20,
                5);
  for (c1_i57 = 0; c1_i57 < 100; c1_i57++) {
    c1_y[c1_i57] = c1_dv11[c1_i57];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_f_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_Gamma;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[100];
  int32_T c1_i58;
  int32_T c1_i59;
  int32_T c1_i60;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_Gamma = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_Gamma), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_b_Gamma);
  c1_i58 = 0;
  for (c1_i59 = 0; c1_i59 < 5; c1_i59++) {
    for (c1_i60 = 0; c1_i60 < 20; c1_i60++) {
      (*(real_T (*)[100])c1_outData)[c1_i60 + c1_i58] = c1_y[c1_i60 + c1_i58];
    }

    c1_i58 += 20;
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_g_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  c1_scZlGNcmSdXsfgNftx2nQBF c1_b_u;
  const mxArray *c1_y = NULL;
  int32_T c1_i61;
  const mxArray *c1_b_y = NULL;
  char_T c1_c_u[6];
  real_T c1_d_u;
  const mxArray *c1_c_y = NULL;
  real_T c1_e_u;
  const mxArray *c1_d_y = NULL;
  boolean_T c1_f_u;
  const mxArray *c1_e_y = NULL;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_b_u = *(c1_scZlGNcmSdXsfgNftx2nQBF *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_createstruct("structure", 2, 1, 1), false);
  for (c1_i61 = 0; c1_i61 < 6; c1_i61++) {
    c1_c_u[c1_i61] = c1_b_u.DataType[c1_i61];
  }

  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_c_u, 10, 0U, 1U, 0U, 2, 1, 6),
                false);
  sf_mex_addfield(c1_y, c1_b_y, "DataType", "DataType", 0);
  c1_d_u = c1_b_u.MaxIter;
  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", &c1_d_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c1_y, c1_c_y, "MaxIter", "MaxIter", 0);
  c1_e_u = c1_b_u.FeasibilityTol;
  c1_d_y = NULL;
  sf_mex_assign(&c1_d_y, sf_mex_create("y", &c1_e_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c1_y, c1_d_y, "FeasibilityTol", "FeasibilityTol", 0);
  c1_f_u = c1_b_u.IntegrityChecks;
  c1_e_y = NULL;
  sf_mex_assign(&c1_e_y, sf_mex_create("y", &c1_f_u, 11, 0U, 0U, 0U, 0), false);
  sf_mex_addfield(c1_y, c1_e_y, "IntegrityChecks", "IntegrityChecks", 0);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static const mxArray *c1_h_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i62;
  int32_T c1_i63;
  const mxArray *c1_y = NULL;
  int32_T c1_i64;
  real_T c1_b_u[400];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_i62 = 0;
  for (c1_i63 = 0; c1_i63 < 20; c1_i63++) {
    for (c1_i64 = 0; c1_i64 < 20; c1_i64++) {
      c1_b_u[c1_i64 + c1_i62] = (*(real_T (*)[400])c1_inData)[c1_i64 + c1_i62];
    }

    c1_i62 += 20;
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 2, 20, 20),
                false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static const mxArray *c1_i_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i65;
  const mxArray *c1_y = NULL;
  real_T c1_b_u[20];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  for (c1_i65 = 0; c1_i65 < 20; c1_i65++) {
    c1_b_u[c1_i65] = (*(real_T (*)[20])c1_inData)[c1_i65];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 1, 20), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static void c1_h_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[20])
{
  real_T c1_dv12[20];
  int32_T c1_i66;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv12, 1, 0, 0U, 1, 0U, 1, 20);
  for (c1_i66 = 0; c1_i66 < 20; c1_i66++) {
    c1_y[c1_i66] = c1_dv12[c1_i66];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_g_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_uMPC;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[20];
  int32_T c1_i67;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_uMPC = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_h_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_uMPC), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_uMPC);
  for (c1_i67 = 0; c1_i67 < 20; c1_i67++) {
    (*(real_T (*)[20])c1_outData)[c1_i67] = c1_y[c1_i67];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_j_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i68;
  const mxArray *c1_y = NULL;
  boolean_T c1_b_u[40];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  for (c1_i68 = 0; c1_i68 < 40; c1_i68++) {
    c1_b_u[c1_i68] = (*(boolean_T (*)[40])c1_inData)[c1_i68];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 11, 0U, 1U, 0U, 1, 40), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static const mxArray *c1_k_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_i69;
  const mxArray *c1_y = NULL;
  real_T c1_b_u[5];
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  for (c1_i69 = 0; c1_i69 < 5; c1_i69++) {
    c1_b_u[c1_i69] = (*(real_T (*)[5])c1_inData)[c1_i69];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 1, 5), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static void c1_i_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[5])
{
  real_T c1_dv13[5];
  int32_T c1_i70;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv13, 1, 0, 0U, 1, 0U, 1, 5);
  for (c1_i70 = 0; c1_i70 < 5; c1_i70++) {
    c1_y[c1_i70] = c1_dv13[c1_i70];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_h_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_kk;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[5];
  int32_T c1_i71;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_kk = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_kk), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_kk);
  for (c1_i71 = 0; c1_i71 < 5; c1_i71++) {
    (*(real_T (*)[5])c1_outData)[c1_i71] = c1_y[c1_i71];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

const mxArray *sf_c1_DoD_simulink_model_2016_get_eml_resolved_functions_info
  (void)
{
  const mxArray *c1_nameCaptureInfo = NULL;
  c1_nameCaptureInfo = NULL;
  sf_mex_assign(&c1_nameCaptureInfo, sf_mex_create("nameCaptureInfo", NULL, 0,
    0U, 1U, 0U, 2, 0, 1), false);
  return c1_nameCaptureInfo;
}

static void c1_mpcqpsolver(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_f[20], real_T c1_A[800], real_T c1_b_b[40], real_T
  c1_x[20], real_T *c1_b_status)
{
  int32_T c1_ii_sizes;
  int32_T c1_tmp_sizes;
  int32_T c1_loop_ub;
  int32_T c1_i72;
  int32_T c1_varargin_1;
  real_T c1_tmp_data[40];
  int32_T c1_ii_data[40];
  int32_T c1_b_varargin_1;
  int32_T c1_n;
  real_T c1_numActive;
  real_T c1_varargin_3;
  real_T c1_alpha1;
  real_T c1_varargin_5;
  real_T c1_beta1;
  const mxArray *c1_y = NULL;
  char_T c1_TRANSB;
  static char_T c1_c_varargin_1[32] = { 'M', 'P', 'C', ':', 'm', 'p', 'c', 'q',
    'p', 's', 'o', 'l', 'v', 'e', 'r', ':', 'I', 'n', 'v', 'a', 'l', 'i', 'd',
    'W', 'a', 'r', 'm', 'S', 't', 'a', 'r', 't' };

  char_T c1_TRANSA;
  const mxArray *c1_b_y = NULL;
  int32_T c1_i73;
  static char_T c1_varargin_2[205] = { 'T', 'h', 'e', ' ', 'n', 'u', 'm', 'b',
    'e', 'r', ' ', 'o', 'f', ' ', 'a', 'c', 't', 'i', 'v', 'e', ' ', 'i', 'n',
    'e', 'q', 'u', 'a', 'l', 'i', 't', 'i', 'e', 's', ' ', 's', 'p', 'e', 'c',
    'i', 'f', 'i', 'e', 'd', ' ', 'b', 'y', ' ', '\"', 'i', 'A', '0', '\"', ' ',
    'i', 's', ' ', '%', 'd', '.', ' ', ' ', 'W', 'i', 't', 'h', ' ', '%', 'd',
    ' ', 'e', 'q', 'u', 'a', 'l', 'i', 't', 'y', ' ', 'c', 'o', 'n', 's', 't',
    'r', 'a', 'i', 'n', 't', 's', ',', ' ', 't', 'h', 'e', ' ', 't', 'o', 't',
    'a', 'l', ' ', 'n', 'u', 'm', 'b', 'e', 'r', ' ', 'o', 'f', ' ', 'a', 'c',
    't', 'i', 'v', 'e', ' ', 'c', 'o', 'n', 's', 't', 'r', 'a', 'i', 'n', 't',
    's', ',', ' ', '%', 'd', ',', ' ', 'e', 'x', 'c', 'e', 'e', 'd', 's', ' ',
    't', 'h', 'e', ' ', 'n', 'u', 'm', 'b', 'e', 'r', ' ', 'o', 'f', ' ', 'v',
    'a', 'r', 'i', 'a', 'b', 'l', 'e', 's', ',', ' ', '%', 'd', ',', ' ', 'm',
    'a', 'k', 'i', 'n', 'g', ' ', 't', 'h', 'e', ' ', 'Q', 'P', ' ', 'p', 'r',
    'o', 'b', 'l', 'e', 'm', ' ', 'i', 'n', 'f', 'e', 'a', 's', 'i', 'b', 'l',
    'e', '.' };

  const mxArray *c1_c_y = NULL;
  ptrdiff_t c1_m_t;
  real_T c1_Hinv[400];
  real_T c1_b_u;
  ptrdiff_t c1_n_t;
  const mxArray *c1_d_y = NULL;
  ptrdiff_t c1_k_t;
  ptrdiff_t c1_lda_t;
  const mxArray *c1_e_y = NULL;
  ptrdiff_t c1_ldb_t;
  ptrdiff_t c1_ldc_t;
  real_T c1_c_u;
  static real_T c1_b_a[400] = { 0.17953118100446194, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -0.35596589217268981, 0.42314360962586206, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.31212981554100422,
    -0.15085880017441863, 0.5395265851382075, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.22675653524786851,
    -0.24895431198843393, -0.014262270707080226, 0.59211926551196981, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    -0.13836709348478629, -0.21205994187589813, -0.18109207087873616,
    0.047328381414249318, 0.61112340659458753, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.065390865873951043,
    -0.1357940312684259, -0.18485281862460215, -0.15268622922557878,
    0.066428855805202885, 0.61609645345964925, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.017153889967133736,
    -0.065402106435703475, -0.12923880337950336, -0.17729389107872212,
    -0.14633752595678243, 0.070789842264748715, 0.61752066298854769, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0078411135547972188,
    -0.017973592955685527, -0.0688067570398097, -0.13199197751842245,
    -0.1781025731904336, -0.14512159360004248, 0.073139856854092244,
    0.618877712158251, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.016792892042678826, 0.006754449033805573, -0.025014422905191645,
    -0.075511091762731422, -0.13587450839489759, -0.17874273757436696,
    -0.14313191294784083, 0.076332096826689511, 0.620588804519038, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.017113889112630693,
    0.015731139145787214, -0.00047862591763447643, -0.032129433380572163,
    -0.080056532210652731, -0.13739387415204857, -0.17757747081585479,
    -0.13998345115540026, 0.080166901496609044, 0.62248419151390333, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.01404679602414171,
    0.016185625920936923, 0.0096940723600378014, -0.0064963725742828932,
    -0.036169443736289414, -0.081770412971494066, -0.13698087464975267,
    -0.1753057517130274, -0.13624480592327431, 0.084335946763323524,
    0.62451257844946806, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.010482631107179853, 0.013258331794745552, 0.011566364650929865,
    0.0050730630574488929, -0.0096980143063973984, -0.037737852126643358,
    -0.081855393455244174, -0.13566229424971757, -0.17253210532067745,
    -0.132123897204966, 0.088881192432794964, 0.626749742358861, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.007609144225287896, 0.0098036329352445932,
    0.0097942556978274555, 0.0081158154215212278, 0.0026362238008596039,
    -0.011019124823261331, -0.038094337555445383, -0.081286692035685393,
    -0.13397358139977011, -0.16940390222489501, -0.12750262621125666,
    0.094031612726143993, 0.629321414091852, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0056503763174164147, 0.0070043049750641261, 0.0071348572763342095,
    0.0071668280151789554, 0.0062504460102224026, 0.0015581893404897267,
    -0.011482825060102213, -0.038015764072695642, -0.080492196616852987,
    -0.13203533829528402, -0.16581111357097594, -0.12211944601290357,
    0.10009345772427337, 0.63238080360261406, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0044191163595166541, 0.0050913460341440427, 0.0048392200597921981,
    0.0050399411987726694, 0.0056942409780984986, 0.0053808926434729936,
    0.001096673102422268, -0.011662981367827761, -0.037831682769763404,
    -0.079581195629996465, -0.12977631038826129, -0.16152819643017485,
    -0.11562337865774583, 0.10745698452253324, 0.63612782549517355, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0036378843765188257, 0.0038871871558543722,
    0.0032452913254430115, 0.0030882386581324041, 0.003836387614978537,
    0.0049980320983291594, 0.0049859139481912883, 0.00082945324616624837,
    -0.011820631184603076, -0.037652580568818823, -0.078530614410585509,
    -0.12703444898697747, -0.15623097718874451, -0.10752667340175709,
    0.11667325035042805, 0.64085382460804885, 0.0, 0.0, 0.0, 0.0,
    0.0030806749308000141, 0.0031226486759438109, 0.0022647328163359392,
    0.0017374311523575998, 0.0020818899277130921, 0.0032855725395688645,
    0.004699451064870676, 0.0047397989263093642, 0.000536667336363688,
    -0.012075207685536573, -0.037505507434398136, -0.077255324545548953,
    -0.1235574443351526, -0.14943043251889698, -0.097082020512002751,
    0.12859974089490123, 0.64702223690479121, 0.0, 0.0, 0.0,
    0.0026078701975466852, 0.0025774372668732233, 0.0016850269629045736,
    0.00095607535964168831, 0.0008919675437983743, 0.001655529200958293,
    0.0030899366817990831, 0.0045303506316400708, 0.0044473630990359064,
    8.8255026121634059E-5, -0.012501775660427579, -0.037390146457159851,
    -0.0756096624066169, -0.11893450275079041, -0.14031711491355309,
    -0.083036421923058462, 0.14468716622648875, 0.6554317041768738, 0.0, 0.0,
    0.0021524874181201204, 0.0021152123431113232, 0.0013143364410911163,
    0.00056105746294453937, 0.00025484061500147552, 0.00057437073779722082,
    0.0015564764276950239, 0.0030184046344605936, 0.00431418933685784,
    0.0039706503037129281, -0.00063570557752505607, -0.013192755931829855,
    -0.037301806748486946, -0.073338786154699, -0.11242510917588491,
    -0.12741027099252486, -0.063080140974317231, 0.16762711643300396,
    0.66759498812525553, 0.0, 0.0026191208278017273, 0.0026019082051462069,
    0.0016216283611085358, 0.00059898831032237419, -4.9249698557432218E-5,
    -0.00015152176150694629, 0.000383387430390087, 0.0015945134104339185,
    0.00333741516032808, 0.004983569800658629, 0.0049863787430134976,
    0.00047065099914092822, -0.012768012332613427, -0.039135056992419641,
    -0.079928086371240215, -0.12674359755585715, -0.15127058736482824,
    -0.093565438726647021, 0.14412109784377947, 0.68298139053534912 };

  static real_T c1_c_b[400] = { 0.17953118100446194, -0.35596589217268981,
    -0.31212981554100422, -0.22675653524786851, -0.13836709348478629,
    -0.065390865873951043, -0.017153889967133736, 0.0078411135547972188,
    0.016792892042678826, 0.017113889112630693, 0.01404679602414171,
    0.010482631107179853, 0.007609144225287896, 0.0056503763174164147,
    0.0044191163595166541, 0.0036378843765188257, 0.0030806749308000141,
    0.0026078701975466852, 0.0021524874181201204, 0.0026191208278017273, 0.0,
    0.42314360962586206, -0.15085880017441863, -0.24895431198843393,
    -0.21205994187589813, -0.1357940312684259, -0.065402106435703475,
    -0.017973592955685527, 0.006754449033805573, 0.015731139145787214,
    0.016185625920936923, 0.013258331794745552, 0.0098036329352445932,
    0.0070043049750641261, 0.0050913460341440427, 0.0038871871558543722,
    0.0031226486759438109, 0.0025774372668732233, 0.0021152123431113232,
    0.0026019082051462069, 0.0, 0.0, 0.5395265851382075, -0.014262270707080226,
    -0.18109207087873616, -0.18485281862460215, -0.12923880337950336,
    -0.0688067570398097, -0.025014422905191645, -0.00047862591763447643,
    0.0096940723600378014, 0.011566364650929865, 0.0097942556978274555,
    0.0071348572763342095, 0.0048392200597921981, 0.0032452913254430115,
    0.0022647328163359392, 0.0016850269629045736, 0.0013143364410911163,
    0.0016216283611085358, 0.0, 0.0, 0.0, 0.59211926551196981,
    0.047328381414249318, -0.15268622922557878, -0.17729389107872212,
    -0.13199197751842245, -0.075511091762731422, -0.032129433380572163,
    -0.0064963725742828932, 0.0050730630574488929, 0.0081158154215212278,
    0.0071668280151789554, 0.0050399411987726694, 0.0030882386581324041,
    0.0017374311523575998, 0.00095607535964168831, 0.00056105746294453937,
    0.00059898831032237419, 0.0, 0.0, 0.0, 0.0, 0.61112340659458753,
    0.066428855805202885, -0.14633752595678243, -0.1781025731904336,
    -0.13587450839489759, -0.080056532210652731, -0.036169443736289414,
    -0.0096980143063973984, 0.0026362238008596039, 0.0062504460102224026,
    0.0056942409780984986, 0.003836387614978537, 0.0020818899277130921,
    0.0008919675437983743, 0.00025484061500147552, -4.9249698557432218E-5, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.61609645345964925, 0.070789842264748715,
    -0.14512159360004248, -0.17874273757436696, -0.13739387415204857,
    -0.081770412971494066, -0.037737852126643358, -0.011019124823261331,
    0.0015581893404897267, 0.0053808926434729936, 0.0049980320983291594,
    0.0032855725395688645, 0.001655529200958293, 0.00057437073779722082,
    -0.00015152176150694629, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.61752066298854769,
    0.073139856854092244, -0.14313191294784083, -0.17757747081585479,
    -0.13698087464975267, -0.081855393455244174, -0.038094337555445383,
    -0.011482825060102213, 0.001096673102422268, 0.0049859139481912883,
    0.004699451064870676, 0.0030899366817990831, 0.0015564764276950239,
    0.000383387430390087, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.618877712158251,
    0.076332096826689511, -0.13998345115540026, -0.1753057517130274,
    -0.13566229424971757, -0.081286692035685393, -0.038015764072695642,
    -0.011662981367827761, 0.00082945324616624837, 0.0047397989263093642,
    0.0045303506316400708, 0.0030184046344605936, 0.0015945134104339185, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.620588804519038, 0.080166901496609044,
    -0.13624480592327431, -0.17253210532067745, -0.13397358139977011,
    -0.080492196616852987, -0.037831682769763404, -0.011820631184603076,
    0.000536667336363688, 0.0044473630990359064, 0.00431418933685784,
    0.00333741516032808, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.62248419151390333, 0.084335946763323524, -0.132123897204966,
    -0.16940390222489501, -0.13203533829528402, -0.079581195629996465,
    -0.037652580568818823, -0.012075207685536573, 8.8255026121634059E-5,
    0.0039706503037129281, 0.004983569800658629, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.62451257844946806, 0.088881192432794964,
    -0.12750262621125666, -0.16581111357097594, -0.12977631038826129,
    -0.078530614410585509, -0.037505507434398136, -0.012501775660427579,
    -0.00063570557752505607, 0.0049863787430134976, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.626749742358861, 0.094031612726143993,
    -0.12211944601290357, -0.16152819643017485, -0.12703444898697747,
    -0.077255324545548953, -0.037390146457159851, -0.013192755931829855,
    0.00047065099914092822, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.629321414091852, 0.10009345772427337, -0.11562337865774583,
    -0.15623097718874451, -0.1235574443351526, -0.0756096624066169,
    -0.037301806748486946, -0.012768012332613427, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.63238080360261406, 0.10745698452253324,
    -0.10752667340175709, -0.14943043251889698, -0.11893450275079041,
    -0.073338786154699, -0.039135056992419641, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.63612782549517355, 0.11667325035042805,
    -0.097082020512002751, -0.14031711491355309, -0.11242510917588491,
    -0.079928086371240215, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.64085382460804885, 0.12859974089490123,
    -0.083036421923058462, -0.12741027099252486, -0.12674359755585715, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.64702223690479121, 0.14468716622648875, -0.063080140974317231,
    -0.15127058736482824, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6554317041768738, 0.16762711643300396,
    -0.093565438726647021, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.66759498812525553, 0.14412109784377947,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.68298139053534912 };

  const mxArray *c1_f_y = NULL;
  int32_T c1_i74;
  int32_T c1_i75;
  c1_cell_0 c1_reshapes;
  int32_T c1_i76;
  real_T c1_d_varargin_1[800];
  int32_T c1_i77;
  real_T c1_b_f[20];
  real_T c1_d_b[40];
  real_T c1_lam[40];
  real_T c1_c_status;
  int16_T c1_iA1[40];
  c1_ii_sizes = 0;
  c1_tmp_sizes = c1_ii_sizes;
  c1_loop_ub = c1_ii_sizes - 1;
  for (c1_i72 = 0; c1_i72 <= c1_loop_ub; c1_i72++) {
    c1_tmp_data[c1_i72] = (real_T)c1_ii_data[c1_i72];
  }

  c1_varargin_1 = c1_tmp_sizes;
  c1_b_varargin_1 = c1_varargin_1;
  c1_n = c1_b_varargin_1;
  c1_numActive = (real_T)c1_n;
  if (c1_numActive > 20.0) {
    c1_varargin_3 = c1_numActive;
    c1_varargin_5 = c1_numActive;
    c1_y = NULL;
    sf_mex_assign(&c1_y, sf_mex_create("y", c1_c_varargin_1, 10, 0U, 1U, 0U, 2,
      1, 32), false);
    c1_b_y = NULL;
    sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_varargin_2, 10, 0U, 1U, 0U, 2,
      1, 205), false);
    c1_c_y = NULL;
    sf_mex_assign(&c1_c_y, sf_mex_create("y", &c1_varargin_3, 0, 0U, 0U, 0U, 0),
                  false);
    c1_b_u = 0.0;
    c1_d_y = NULL;
    sf_mex_assign(&c1_d_y, sf_mex_create("y", &c1_b_u, 0, 0U, 0U, 0U, 0), false);
    c1_e_y = NULL;
    sf_mex_assign(&c1_e_y, sf_mex_create("y", &c1_varargin_5, 0, 0U, 0U, 0U, 0),
                  false);
    c1_c_u = 20.0;
    c1_f_y = NULL;
    sf_mex_assign(&c1_f_y, sf_mex_create("y", &c1_c_u, 0, 0U, 0U, 0U, 0), false);
    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 6U, 14, c1_y, 14,
                      c1_b_y, 14, c1_c_y, 14, c1_d_y, 14, c1_e_y, 14, c1_f_y);
  }

  c1_alpha1 = 1.0;
  c1_beta1 = 0.0;
  c1_TRANSB = 'N';
  c1_TRANSA = 'N';
  for (c1_i73 = 0; c1_i73 < 400; c1_i73++) {
    c1_Hinv[c1_i73] = 0.0;
  }

  c1_m_t = (ptrdiff_t)20;
  c1_n_t = (ptrdiff_t)20;
  c1_k_t = (ptrdiff_t)20;
  c1_lda_t = (ptrdiff_t)20;
  c1_ldb_t = (ptrdiff_t)20;
  c1_ldc_t = (ptrdiff_t)20;
  dgemm(&c1_TRANSA, &c1_TRANSB, &c1_m_t, &c1_n_t, &c1_k_t, &c1_alpha1, &c1_b_a[0],
        &c1_lda_t, &c1_c_b[0], &c1_ldb_t, &c1_beta1, &c1_Hinv[0], &c1_ldc_t);
  for (c1_i74 = 0; c1_i74 < 800; c1_i74++) {
    c1_reshapes.f1[c1_i74] = c1_A[c1_i74];
  }

  for (c1_i75 = 0; c1_i75 < 800; c1_i75++) {
    c1_d_varargin_1[c1_i75] = c1_reshapes.f1[c1_i75];
  }

  for (c1_i76 = 0; c1_i76 < 20; c1_i76++) {
    c1_b_f[c1_i76] = c1_f[c1_i76];
  }

  for (c1_i77 = 0; c1_i77 < 40; c1_i77++) {
    c1_d_b[c1_i77] = c1_b_b[c1_i77];
  }

  c1_qpkwik(chartInstance, c1_Hinv, c1_b_f, c1_d_varargin_1, c1_d_b, 200, 1.0E-6,
            c1_x, c1_lam, &c1_c_status, c1_iA1);
  *c1_b_status = c1_c_status;
}

static void c1_check_forloop_overflow_error
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, boolean_T
   c1_overflow)
{
  const mxArray *c1_y = NULL;
  static char_T c1_cv0[34] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'i', 'n', 't', '_', 'f', 'o', 'r', 'l', 'o', 'o', 'p',
    '_', 'o', 'v', 'e', 'r', 'f', 'l', 'o', 'w' };

  const mxArray *c1_b_y = NULL;
  static char_T c1_cv1[5] = { 'i', 'n', 't', '3', '2' };

  (void)chartInstance;
  (void)c1_overflow;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_cv0, 10, 0U, 1U, 0U, 2, 1, 34),
                false);
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_cv1, 10, 0U, 1U, 0U, 2, 1, 5),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    2U, 14, c1_y, 14, c1_b_y));
}

static void c1_qpkwik(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                      real_T c1_Hinv[400], real_T c1_f[20], real_T c1_Ac[800],
                      real_T c1_b_b[40], int16_T c1_maxiter, real_T c1_FeasTol,
                      real_T c1_x[20], real_T c1_lambda[40], real_T *c1_b_status,
                      int16_T c1_iA[40])
{
  int32_T c1_i78;
  int32_T c1_i79;
  int32_T c1_i80;
  real_T c1_rMin;
  real_T c1_r[20];
  int32_T c1_i81;
  boolean_T c1_cTolComputed;
  real_T c1_cTol[40];
  int32_T c1_i82;
  int16_T c1_nA;
  int16_T c1_iC[40];
  int32_T c1_i83;
  int32_T c1_i84;
  int32_T c1_i85;
  real_T c1_b_Hinv[400];
  real_T c1_b_f[20];
  int32_T c1_i86;
  real_T c1_Xnorm0;
  real_T c1_b_x[20];
  real_T c1_cMin;
  int16_T c1_kNext;
  int16_T c1_i;
  int32_T c1_b_i;
  int32_T c1_i87;
  int32_T c1_c_i;
  int32_T c1_i88;
  int32_T c1_i89;
  int32_T c1_b_kNext;
  real_T c1_b_Ac[20];
  real_T c1_AcRow[20];
  int32_T c1_i90;
  int32_T c1_ixstart;
  real_T c1_y;
  real_T c1_b_a[20];
  real_T c1_mtmp;
  real_T c1_Xnorm;
  real_T c1_c_x[20];
  int32_T c1_k;
  real_T c1_d_x;
  real_T c1_e_x;
  boolean_T c1_foundnan;
  real_T c1_f_x;
  int32_T c1_i91;
  int32_T c1_i92;
  real_T c1_g_x;
  real_T c1_A;
  int32_T c1_ix;
  real_T c1_b_y;
  real_T c1_B;
  int32_T c1_i93;
  real_T c1_c_Ac[800];
  int32_T c1_i94;
  real_T c1_c_b[20];
  real_T c1_h_x;
  int32_T c1_i95;
  real_T c1_c_y;
  real_T c1_maxval;
  int32_T c1_b_ix;
  int32_T c1_i96;
  real_T c1_i_x;
  real_T c1_varargin_1;
  real_T c1_j_x;
  real_T c1_Stat;
  int16_T c1_b_iC[40];
  real_T c1_RLinv[400];
  real_T c1_D[400];
  real_T c1_H[400];
  real_T c1_z[20];
  real_T c1_d_y;
  real_T c1_varargin_2;
  boolean_T c1_d_b;
  int16_T c1_kDrop;
  int32_T c1_i97;
  real_T c1_cVal;
  real_T c1_b_varargin_2;
  real_T c1_c_a;
  real_T c1_e_b[40];
  real_T c1_b_varargin_1[40];
  int32_T c1_i98;
  real_T c1_t1;
  int32_T c1_i99;
  real_T c1_varargin_3;
  real_T c1_f_b;
  int32_T c1_b_k;
  boolean_T c1_isT1Inf;
  real_T c1_k_x;
  boolean_T c1_p;
  boolean_T c1_tempOK;
  real_T c1_e_y;
  int32_T c1_i100;
  real_T c1_d_a[400];
  real_T c1_l_x;
  real_T c1_xk;
  int16_T c1_i101;
  real_T c1_f_y;
  real_T c1_m_x;
  int16_T c1_g_b;
  real_T c1_n_x;
  real_T c1_extremum;
  int32_T c1_i102;
  int16_T c1_h_b;
  real_T c1_g_y;
  boolean_T c1_b0;
  boolean_T c1_overflow;
  real_T c1_b_maxval;
  int16_T c1_i_b;
  int16_T c1_j_b;
  int32_T c1_i103;
  int16_T c1_i104;
  int16_T c1_ct;
  boolean_T c1_b_overflow;
  int32_T c1_i105;
  int32_T c1_i106;
  int16_T c1_k_b;
  int16_T c1_l_b;
  boolean_T c1_c_overflow;
  int16_T c1_d_i;
  int32_T c1_i107;
  int16_T c1_e_i;
  int32_T c1_f_i;
  real_T c1_zTa;
  int32_T c1_i108;
  int32_T c1_c_k;
  real_T c1_b_A;
  real_T c1_h_y;
  real_T c1_b_B;
  int32_T c1_d_k;
  real_T c1_i_y;
  real_T c1_t2;
  real_T c1_o_x;
  int32_T c1_e_k;
  boolean_T c1_isT2Inf;
  real_T c1_j_y;
  real_T c1_p_x;
  real_T c1_k_y;
  real_T c1_c_A;
  real_T c1_rVal;
  real_T c1_c_B;
  real_T c1_q_x;
  real_T c1_t;
  real_T c1_l_y;
  real_T c1_r_x;
  int16_T c1_m_b;
  real_T c1_m_y;
  int16_T c1_n_b;
  boolean_T c1_d_overflow;
  int16_T c1_g_i;
  int16_T c1_f_k;
  int16_T c1_b_nA;
  real_T c1_e_a;
  int32_T c1_i109;
  int32_T c1_i110;
  int32_T c1_i111;
  int16_T c1_h_i;
  int32_T c1_i112;
  int16_T c1_iSave;
  int32_T c1_i113;
  int32_T c1_i114;
  int32_T exitg1;
  int32_T exitg2;
  boolean_T exitg3;
  boolean_T exitg4;
  boolean_T exitg5;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  (void)c1_maxiter;
  (void)c1_FeasTol;
  for (c1_i78 = 0; c1_i78 < 40; c1_i78++) {
    c1_iA[c1_i78] = 0;
  }

  *c1_b_status = 1.0;
  for (c1_i79 = 0; c1_i79 < 40; c1_i79++) {
    c1_lambda[c1_i79] = 0.0;
  }

  for (c1_i80 = 0; c1_i80 < 20; c1_i80++) {
    c1_r[c1_i80] = 0.0;
  }

  c1_rMin = 0.0;
  for (c1_i81 = 0; c1_i81 < 40; c1_i81++) {
    c1_cTol[c1_i81] = 1.0;
  }

  c1_cTolComputed = false;
  for (c1_i82 = 0; c1_i82 < 40; c1_i82++) {
    c1_iC[c1_i82] = 0;
  }

  c1_nA = 0;
  for (c1_i83 = 0; c1_i83 < 20; c1_i83++) {
    c1_x[c1_i83] = 0.0;
  }

  for (c1_i84 = 0; c1_i84 < 400; c1_i84++) {
    c1_b_Hinv[c1_i84] = c1_Hinv[c1_i84];
  }

  for (c1_i85 = 0; c1_i85 < 20; c1_i85++) {
    c1_b_f[c1_i85] = c1_f[c1_i85];
  }

  c1_b_Unconstrained(chartInstance, c1_b_Hinv, c1_b_f, c1_x, 20);
  for (c1_i86 = 0; c1_i86 < 20; c1_i86++) {
    c1_b_x[c1_i86] = c1_x[c1_i86];
  }

  c1_Xnorm0 = c1_norm(chartInstance, c1_b_x);
  do {
    exitg2 = 0;
    if (*c1_b_status <= 200.0) {
      c1_cMin = -1.0E-6;
      c1_kNext = 0;
      for (c1_i = 1; c1_i < 41; c1_i++) {
        if (!c1_cTolComputed) {
          c1_b_i = c1_i - 1;
          for (c1_i87 = 0; c1_i87 < 20; c1_i87++) {
            c1_b_Ac[c1_i87] = c1_Ac[c1_b_i + 40 * c1_i87] * c1_x[c1_i87];
          }

          c1_abs(chartInstance, c1_b_Ac, c1_AcRow);
          c1_ixstart = 1;
          c1_mtmp = c1_AcRow[0];
          c1_d_x = c1_mtmp;
          c1_foundnan = muDoubleScalarIsNaN(c1_d_x);
          if (c1_foundnan) {
            c1_ix = 1;
            exitg5 = false;
            while ((!exitg5) && (c1_ix + 1 < 21)) {
              c1_ixstart = c1_ix + 1;
              c1_j_x = c1_AcRow[c1_ix];
              c1_d_b = muDoubleScalarIsNaN(c1_j_x);
              if (!c1_d_b) {
                c1_mtmp = c1_AcRow[c1_ix];
                exitg5 = true;
              } else {
                c1_ix++;
              }
            }
          }

          if (c1_ixstart < 20) {
            c1_i95 = c1_ixstart;
            for (c1_b_ix = c1_i95; c1_b_ix + 1 < 21; c1_b_ix++) {
              c1_c_a = c1_AcRow[c1_b_ix];
              c1_f_b = c1_mtmp;
              c1_p = (c1_c_a > c1_f_b);
              if (c1_p) {
                c1_mtmp = c1_AcRow[c1_b_ix];
              }
            }
          }

          c1_maxval = c1_mtmp;
          c1_varargin_1 = c1_cTol[c1_i - 1];
          c1_varargin_2 = c1_maxval;
          c1_b_varargin_2 = c1_varargin_1;
          c1_varargin_3 = c1_varargin_2;
          c1_k_x = c1_b_varargin_2;
          c1_e_y = c1_varargin_3;
          c1_l_x = c1_k_x;
          c1_f_y = c1_e_y;
          c1_n_x = c1_l_x;
          c1_g_y = c1_f_y;
          c1_b_maxval = muDoubleScalarMax(c1_n_x, c1_g_y);
          c1_cTol[c1_i - 1] = c1_b_maxval;
        }

        if (c1_iA[c1_i - 1] == 0) {
          c1_c_i = c1_i - 1;
          for (c1_i88 = 0; c1_i88 < 20; c1_i88++) {
            c1_b_a[c1_i88] = c1_Ac[c1_c_i + 40 * c1_i88];
          }

          c1_y = 0.0;
          for (c1_k = 0; c1_k + 1 < 21; c1_k++) {
            c1_y += c1_b_a[c1_k] * c1_x[c1_k];
          }

          c1_A = c1_y - c1_b_b[c1_i - 1];
          c1_B = c1_cTol[c1_i - 1];
          c1_h_x = c1_A;
          c1_c_y = c1_B;
          c1_i_x = c1_h_x;
          c1_d_y = c1_c_y;
          c1_cVal = c1_i_x / c1_d_y;
          if (c1_cVal < c1_cMin) {
            c1_cMin = c1_cVal;
            c1_kNext = c1_i;
          }
        }
      }

      c1_cTolComputed = true;
      if (c1_kNext <= 0) {
        exitg2 = 1;
      } else {
        do {
          exitg1 = 0;
          if ((c1_kNext > 0) && (*c1_b_status <= 200.0)) {
            c1_b_kNext = c1_kNext - 1;
            for (c1_i90 = 0; c1_i90 < 20; c1_i90++) {
              c1_AcRow[c1_i90] = c1_Ac[c1_b_kNext + 40 * c1_i90];
            }

            guard1 = false;
            guard2 = false;
            if (c1_nA == 0) {
              for (c1_i92 = 0; c1_i92 < 20; c1_i92++) {
                c1_c_b[c1_i92] = c1_AcRow[c1_i92];
              }

              for (c1_i94 = 0; c1_i94 < 20; c1_i94++) {
                c1_z[c1_i94] = 0.0;
                c1_i97 = 0;
                for (c1_i99 = 0; c1_i99 < 20; c1_i99++) {
                  c1_z[c1_i94] += c1_Hinv[c1_i97 + c1_i94] * c1_c_b[c1_i99];
                  c1_i97 += 20;
                }
              }

              guard2 = true;
            } else {
              for (c1_i91 = 0; c1_i91 < 800; c1_i91++) {
                c1_c_Ac[c1_i91] = c1_Ac[c1_i91];
              }

              for (c1_i93 = 0; c1_i93 < 40; c1_i93++) {
                c1_b_iC[c1_i93] = c1_iC[c1_i93];
              }

              c1_Stat = c1_b_KWIKfactor(chartInstance, c1_c_Ac, c1_b_iC, c1_nA,
                c1_RLinv, c1_D, c1_H);
              if (c1_Stat <= 0.0) {
                *c1_b_status = -2.0;
                exitg1 = 1;
              } else {
                for (c1_i98 = 0; c1_i98 < 400; c1_i98++) {
                  c1_d_a[c1_i98] = -c1_H[c1_i98];
                }

                for (c1_i100 = 0; c1_i100 < 20; c1_i100++) {
                  c1_c_b[c1_i100] = c1_AcRow[c1_i100];
                }

                for (c1_i102 = 0; c1_i102 < 20; c1_i102++) {
                  c1_z[c1_i102] = 0.0;
                  c1_i103 = 0;
                  for (c1_i105 = 0; c1_i105 < 20; c1_i105++) {
                    c1_z[c1_i102] += c1_d_a[c1_i103 + c1_i102] * c1_c_b[c1_i105];
                    c1_i103 += 20;
                  }
                }

                c1_i_b = c1_nA;
                c1_j_b = c1_i_b;
                c1_b_overflow = ((!(1 > c1_j_b)) && (c1_j_b > 32766));
                if (c1_b_overflow) {
                  c1_b_check_forloop_overflow_error(chartInstance, true);
                }

                for (c1_d_i = 1; c1_d_i <= c1_nA; c1_d_i++) {
                  c1_f_i = (int16_T)sf_eml_array_bounds_check
                    (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                     MAX_uint32_T, (int32_T)c1_d_i, 1, 20) - 1;
                  for (c1_i108 = 0; c1_i108 < 20; c1_i108++) {
                    c1_c_b[c1_i108] = c1_D[c1_i108 + 20 * c1_f_i];
                  }

                  c1_h_y = 0.0;
                  for (c1_d_k = 0; c1_d_k + 1 < 21; c1_d_k++) {
                    c1_h_y += c1_AcRow[c1_d_k] * c1_c_b[c1_d_k];
                  }

                  c1_r[(int16_T)sf_eml_array_bounds_check
                    (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                     MAX_uint32_T, (int32_T)c1_d_i, 1, 20) - 1] = c1_h_y;
                }

                guard2 = true;
              }
            }

            if (guard2) {
              c1_kDrop = 0;
              c1_t1 = 0.0;
              c1_isT1Inf = true;
              c1_tempOK = true;
              if (c1_nA > 0) {
                c1_i101 = c1_nA;
                c1_g_b = c1_i101;
                c1_h_b = c1_g_b;
                c1_overflow = ((!(1 > c1_h_b)) && (c1_h_b > 32766));
                if (c1_overflow) {
                  c1_b_check_forloop_overflow_error(chartInstance, true);
                }

                c1_ct = 1;
                exitg4 = false;
                while ((!exitg4) && (c1_ct <= c1_i101)) {
                  if (c1_r[(int16_T)sf_eml_array_bounds_check
                      (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                       MAX_uint32_T, (int32_T)c1_ct, 1, 20) - 1] >= 1.0E-12) {
                    c1_tempOK = false;
                    exitg4 = true;
                  } else {
                    c1_ct++;
                  }
                }
              }

              if ((c1_nA == 0) || c1_tempOK) {
                c1_b0 = true;
              } else {
                c1_b0 = false;
              }

              if (!c1_b0) {
                c1_i104 = c1_nA;
                c1_k_b = c1_i104;
                c1_l_b = c1_k_b;
                c1_c_overflow = ((!(1 > c1_l_b)) && (c1_l_b > 32766));
                if (c1_c_overflow) {
                  c1_b_check_forloop_overflow_error(chartInstance, true);
                }

                for (c1_e_i = 1; c1_e_i <= c1_i104; c1_e_i++) {
                  if (c1_r[(int16_T)sf_eml_array_bounds_check
                      (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                       MAX_uint32_T, (int32_T)c1_e_i, 1, 20) - 1] > 1.0E-12) {
                    c1_b_A = c1_lambda[(int16_T)sf_eml_array_bounds_check
                      (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                       MAX_uint32_T, (int32_T)c1_iC[c1_e_i - 1], 1, 40) - 1];
                    c1_b_B = c1_r[c1_e_i - 1];
                    c1_o_x = c1_b_A;
                    c1_j_y = c1_b_B;
                    c1_p_x = c1_o_x;
                    c1_k_y = c1_j_y;
                    c1_rVal = c1_p_x / c1_k_y;
                    if ((c1_kDrop == 0) || (c1_rVal < c1_rMin)) {
                      c1_rMin = c1_rVal;
                      c1_kDrop = c1_e_i;
                    }
                  }
                }

                if (c1_kDrop > 0) {
                  c1_t1 = c1_rMin;
                  c1_isT1Inf = false;
                }
              }

              for (c1_i106 = 0; c1_i106 < 20; c1_i106++) {
                c1_b_a[c1_i106] = c1_z[c1_i106];
              }

              for (c1_i107 = 0; c1_i107 < 20; c1_i107++) {
                c1_c_b[c1_i107] = c1_AcRow[c1_i107];
              }

              c1_zTa = 0.0;
              for (c1_c_k = 0; c1_c_k + 1 < 21; c1_c_k++) {
                c1_zTa += c1_b_a[c1_c_k] * c1_c_b[c1_c_k];
              }

              if (c1_zTa <= 0.0) {
                c1_t2 = 0.0;
                c1_isT2Inf = true;
              } else {
                c1_i_y = 0.0;
                for (c1_e_k = 0; c1_e_k + 1 < 21; c1_e_k++) {
                  c1_i_y += c1_AcRow[c1_e_k] * c1_x[c1_e_k];
                }

                c1_c_A = c1_b_b[c1_kNext - 1] - c1_i_y;
                c1_c_B = c1_zTa;
                c1_q_x = c1_c_A;
                c1_l_y = c1_c_B;
                c1_r_x = c1_q_x;
                c1_m_y = c1_l_y;
                c1_t2 = c1_r_x / c1_m_y;
                c1_isT2Inf = false;
              }

              if (c1_isT1Inf && c1_isT2Inf) {
                *c1_b_status = -1.0;
                exitg1 = 1;
              } else {
                if (c1_isT2Inf) {
                  c1_t = c1_t1;
                } else if (c1_isT1Inf) {
                  c1_t = c1_t2;
                } else {
                  c1_t = muDoubleScalarMin(c1_t1, c1_t2);
                }

                c1_m_b = c1_nA;
                c1_n_b = c1_m_b;
                c1_d_overflow = ((!(1 > c1_n_b)) && (c1_n_b > 32766));
                if (c1_d_overflow) {
                  c1_b_check_forloop_overflow_error(chartInstance, true);
                }

                for (c1_g_i = 1; c1_g_i <= c1_nA; c1_g_i++) {
                  c1_f_k = c1_iC[(int16_T)sf_eml_array_bounds_check
                    (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                     MAX_uint32_T, (int32_T)c1_g_i, 1, 40) - 1];
                  c1_lambda[(int16_T)sf_eml_array_bounds_check
                    (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                     MAX_uint32_T, (int32_T)c1_f_k, 1, 40) - 1] = c1_lambda
                    [(int16_T)sf_eml_array_bounds_check
                    (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                     MAX_uint32_T, (int32_T)c1_f_k, 1, 40) - 1] - c1_t * c1_r
                    [(int16_T)sf_eml_array_bounds_check
                    (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                     MAX_uint32_T, (int32_T)c1_g_i, 1, 20) - 1];
                  if (c1_lambda[c1_f_k - 1] < 0.0) {
                    c1_lambda[c1_f_k - 1] = 0.0;
                  }
                }

                c1_lambda[c1_kNext - 1] += c1_t;
                if (c1_t == c1_t1) {
                  c1_b_nA = c1_nA;
                  c1_b_DropConstraint(chartInstance, c1_kDrop, c1_iA, &c1_b_nA,
                                      c1_iC);
                  c1_nA = c1_b_nA;
                }

                if (!c1_isT2Inf) {
                  c1_e_a = c1_t;
                  for (c1_i109 = 0; c1_i109 < 20; c1_i109++) {
                    c1_z[c1_i109] *= c1_e_a;
                  }

                  for (c1_i110 = 0; c1_i110 < 20; c1_i110++) {
                    c1_x[c1_i110] += c1_z[c1_i110];
                  }

                  if (c1_t == c1_t2) {
                    if (c1_nA == 20) {
                      *c1_b_status = -1.0;
                      exitg1 = 1;
                    } else {
                      c1_i111 = c1_nA + 1;
                      if (c1_i111 > 32767) {
                        c1_i111 = 32767;
                      } else {
                        if (c1_i111 < -32768) {
                          c1_i111 = -32768;
                        }
                      }

                      c1_nA = (int16_T)c1_i111;
                      c1_iC[(int16_T)sf_eml_array_bounds_check
                        (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                         MAX_uint32_T, (int32_T)c1_nA, 1, 40) - 1] = c1_kNext;
                      c1_h_i = c1_nA;
                      exitg3 = false;
                      while ((!exitg3) && (c1_h_i > 1)) {
                        c1_i112 = c1_h_i - 1;
                        if (c1_i112 > 32767) {
                          c1_i112 = 32767;
                        } else {
                          if (c1_i112 < -32768) {
                            c1_i112 = -32768;
                          }
                        }

                        if (c1_iC[c1_h_i - 1] > c1_iC[(int16_T)c1_i112 - 1]) {
                          exitg3 = true;
                        } else {
                          c1_iSave = c1_iC[c1_h_i - 1];
                          c1_i113 = c1_h_i - 1;
                          if (c1_i113 > 32767) {
                            c1_i113 = 32767;
                          } else {
                            if (c1_i113 < -32768) {
                              c1_i113 = -32768;
                            }
                          }

                          c1_iC[c1_h_i - 1] = c1_iC[(int16_T)c1_i113 - 1];
                          c1_i114 = c1_h_i - 1;
                          if (c1_i114 > 32767) {
                            c1_i114 = 32767;
                          } else {
                            if (c1_i114 < -32768) {
                              c1_i114 = -32768;
                            }
                          }

                          c1_iC[(int16_T)c1_i114 - 1] = c1_iSave;
                          c1_h_i--;
                        }
                      }

                      c1_iA[c1_kNext - 1] = 1;
                      c1_kNext = 0;
                      guard1 = true;
                    }
                  } else {
                    guard1 = true;
                  }
                } else {
                  guard1 = true;
                }
              }
            }

            if (guard1) {
              (*c1_b_status)++;
            }
          } else {
            for (c1_i89 = 0; c1_i89 < 20; c1_i89++) {
              c1_c_x[c1_i89] = c1_x[c1_i89];
            }

            c1_Xnorm = c1_norm(chartInstance, c1_c_x);
            c1_e_x = c1_Xnorm - c1_Xnorm0;
            c1_f_x = c1_e_x;
            c1_g_x = c1_f_x;
            c1_b_y = muDoubleScalarAbs(c1_g_x);
            if (c1_b_y > 0.001) {
              c1_Xnorm0 = c1_Xnorm;
              for (c1_i96 = 0; c1_i96 < 40; c1_i96++) {
                c1_e_b[c1_i96] = c1_b_b[c1_i96];
              }

              c1_b_abs(chartInstance, c1_e_b, c1_b_varargin_1);
              for (c1_b_k = 0; c1_b_k + 1 < 41; c1_b_k++) {
                c1_xk = c1_b_varargin_1[c1_b_k];
                c1_m_x = c1_xk;
                c1_extremum = muDoubleScalarMax(c1_m_x, 1.0);
                c1_cTol[c1_b_k] = c1_extremum;
              }

              c1_cTolComputed = false;
            }

            exitg1 = 2;
          }
        } while (exitg1 == 0);

        if (exitg1 == 1) {
          exitg2 = 1;
        }
      }
    } else {
      *c1_b_status = 0.0;
      exitg2 = 1;
    }
  } while (exitg2 == 0);
}

static void c1_KWIKfactor(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Ac[800], int16_T c1_iC[40], int16_T c1_nA, real_T
  c1_RLinv[400], real_T c1_D[400], real_T c1_H[400], real_T c1_b_RLinv[400],
  real_T c1_b_D[400], real_T c1_b_H[400], real_T *c1_Status)
{
  int32_T c1_i115;
  int32_T c1_i116;
  int32_T c1_i117;
  int32_T c1_i118;
  int32_T c1_i119;
  real_T c1_b_Ac[800];
  int16_T c1_b_iC[40];
  for (c1_i115 = 0; c1_i115 < 400; c1_i115++) {
    c1_b_RLinv[c1_i115] = c1_RLinv[c1_i115];
  }

  for (c1_i116 = 0; c1_i116 < 400; c1_i116++) {
    c1_b_D[c1_i116] = c1_D[c1_i116];
  }

  for (c1_i117 = 0; c1_i117 < 400; c1_i117++) {
    c1_b_H[c1_i117] = c1_H[c1_i117];
  }

  for (c1_i118 = 0; c1_i118 < 800; c1_i118++) {
    c1_b_Ac[c1_i118] = c1_Ac[c1_i118];
  }

  for (c1_i119 = 0; c1_i119 < 40; c1_i119++) {
    c1_b_iC[c1_i119] = c1_iC[c1_i119];
  }

  *c1_Status = c1_b_KWIKfactor(chartInstance, c1_b_Ac, c1_b_iC, c1_nA,
    c1_b_RLinv, c1_b_D, c1_b_H);
}

static void c1_b_check_forloop_overflow_error
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, boolean_T
   c1_overflow)
{
  const mxArray *c1_y = NULL;
  static char_T c1_cv2[34] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'i', 'n', 't', '_', 'f', 'o', 'r', 'l', 'o', 'o', 'p',
    '_', 'o', 'v', 'e', 'r', 'f', 'l', 'o', 'w' };

  const mxArray *c1_b_y = NULL;
  static char_T c1_cv3[5] = { 'i', 'n', 't', '1', '6' };

  (void)chartInstance;
  (void)c1_overflow;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_cv2, 10, 0U, 1U, 0U, 2, 1, 34),
                false);
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_cv3, 10, 0U, 1U, 0U, 2, 1, 5),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    2U, 14, c1_y, 14, c1_b_y));
}

static void c1_qr(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                  real_T c1_A[400], real_T c1_Q[400], real_T c1_R[400])
{
  int32_T c1_i120;
  real_T c1_b_A[400];
  real_T c1_tau[20];
  int32_T c1_j;
  ptrdiff_t c1_m_t;
  int32_T c1_b_b;
  ptrdiff_t c1_n_t;
  int32_T c1_c_b;
  ptrdiff_t c1_k_t;
  boolean_T c1_overflow;
  ptrdiff_t c1_lda_t;
  ptrdiff_t c1_info_t;
  int32_T c1_info;
  int32_T c1_i;
  int32_T c1_b_info;
  int32_T c1_c_info;
  boolean_T c1_p;
  int32_T c1_b_a;
  int32_T c1_i121;
  boolean_T c1_b_p;
  int32_T c1_b_i;
  int32_T c1_val;
  boolean_T c1_c_p;
  int32_T c1_i122;
  int32_T c1_b_j;
  int32_T c1_c_i;
  for (c1_i120 = 0; c1_i120 < 400; c1_i120++) {
    c1_b_A[c1_i120] = c1_A[c1_i120];
  }

  c1_b_xgeqrf(chartInstance, c1_b_A, c1_tau);
  for (c1_j = 1; c1_j < 21; c1_j++) {
    c1_b_b = c1_j;
    c1_c_b = c1_b_b;
    c1_overflow = ((!(1 > c1_c_b)) && (c1_c_b > 2147483646));
    if (c1_overflow) {
      c1_check_forloop_overflow_error(chartInstance, true);
    }

    for (c1_i = 0; c1_i + 1 <= c1_j; c1_i++) {
      c1_R[c1_i + 20 * (c1_j - 1)] = c1_b_A[c1_i + 20 * (c1_j - 1)];
    }

    c1_b_a = c1_j;
    c1_i121 = c1_b_a;
    for (c1_b_i = c1_i121 + 1; c1_b_i < 21; c1_b_i++) {
      c1_R[(c1_b_i + 20 * (c1_j - 1)) - 1] = 0.0;
    }
  }

  c1_m_t = (ptrdiff_t)20;
  c1_n_t = (ptrdiff_t)20;
  c1_k_t = (ptrdiff_t)20;
  c1_lda_t = (ptrdiff_t)20;
  c1_info_t = LAPACKE_dorgqr(102, c1_m_t, c1_n_t, c1_k_t, &c1_b_A[0], c1_lda_t,
    &c1_tau[0]);
  c1_info = (int32_T)c1_info_t;
  c1_b_info = c1_info;
  c1_c_info = c1_b_info;
  c1_p = (c1_c_info != 0);
  if (c1_p) {
    c1_b_p = true;
    c1_val = c1_b_info;
    c1_c_p = false;
    if (c1_val == -7) {
      c1_c_p = true;
    } else {
      if (c1_val == -5) {
        c1_c_p = true;
      }
    }

    if (!c1_c_p) {
      if (c1_b_info == -1010) {
        c1_error(chartInstance);
      } else {
        c1_c_error(chartInstance, c1_b_info);
      }
    }
  } else {
    c1_b_p = false;
  }

  if (c1_b_p) {
    for (c1_i122 = 0; c1_i122 < 400; c1_i122++) {
      c1_b_A[c1_i122] = rtNaN;
    }
  }

  for (c1_b_j = 0; c1_b_j + 1 < 21; c1_b_j++) {
    for (c1_c_i = 0; c1_c_i + 1 < 21; c1_c_i++) {
      c1_Q[c1_c_i + 20 * c1_b_j] = c1_b_A[c1_c_i + 20 * c1_b_j];
    }
  }
}

static void c1_xgeqrf(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                      real_T c1_A[400], real_T c1_b_A[400], real_T c1_tau[20])
{
  int32_T c1_i123;
  for (c1_i123 = 0; c1_i123 < 400; c1_i123++) {
    c1_b_A[c1_i123] = c1_A[c1_i123];
  }

  c1_b_xgeqrf(chartInstance, c1_b_A, c1_tau);
}

static void c1_error(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance)
{
  const mxArray *c1_y = NULL;
  static char_T c1_cv4[12] = { 'M', 'A', 'T', 'L', 'A', 'B', ':', 'n', 'o', 'm',
    'e', 'm' };

  (void)chartInstance;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_cv4, 10, 0U, 1U, 0U, 2, 1, 12),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    1U, 14, c1_y));
}

static void c1_b_error(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
  int32_T c1_varargin_2)
{
  const mxArray *c1_y = NULL;
  static char_T c1_cv5[33] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'L', 'A', 'P', 'A', 'C', 'K', 'C', 'a', 'l', 'l', 'E',
    'r', 'r', 'o', 'r', 'I', 'n', 'f', 'o' };

  const mxArray *c1_b_y = NULL;
  static char_T c1_cv6[14] = { 'L', 'A', 'P', 'A', 'C', 'K', 'E', '_', 'd', 'g',
    'e', 'q', 'r', 'f' };

  const mxArray *c1_c_y = NULL;
  (void)chartInstance;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_cv5, 10, 0U, 1U, 0U, 2, 1, 33),
                false);
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_cv6, 10, 0U, 1U, 0U, 2, 1, 14),
                false);
  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", &c1_varargin_2, 6, 0U, 0U, 0U, 0),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    3U, 14, c1_y, 14, c1_b_y, 14, c1_c_y));
}

static void c1_c_error(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
  int32_T c1_varargin_2)
{
  const mxArray *c1_y = NULL;
  static char_T c1_cv7[33] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'L', 'A', 'P', 'A', 'C', 'K', 'C', 'a', 'l', 'l', 'E',
    'r', 'r', 'o', 'r', 'I', 'n', 'f', 'o' };

  const mxArray *c1_b_y = NULL;
  static char_T c1_cv8[14] = { 'L', 'A', 'P', 'A', 'C', 'K', 'E', '_', 'd', 'o',
    'r', 'g', 'q', 'r' };

  const mxArray *c1_c_y = NULL;
  (void)chartInstance;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_cv7, 10, 0U, 1U, 0U, 2, 1, 33),
                false);
  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_cv8, 10, 0U, 1U, 0U, 2, 1, 14),
                false);
  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", &c1_varargin_2, 6, 0U, 0U, 0U, 0),
                false);
  sf_mex_call_debug(sfGlobalDebugInstanceStruct, "error", 0U, 1U, 14,
                    sf_mex_call_debug(sfGlobalDebugInstanceStruct, "message", 1U,
    3U, 14, c1_y, 14, c1_b_y, 14, c1_c_y));
}

static void c1_DropConstraint(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, int16_T c1_kDrop, int16_T c1_iA[40], int16_T c1_nA, int16_T
  c1_iC[40], int16_T c1_b_iA[40], int16_T *c1_b_nA, int16_T c1_b_iC[40])
{
  int32_T c1_i124;
  int32_T c1_i125;
  for (c1_i124 = 0; c1_i124 < 40; c1_i124++) {
    c1_b_iA[c1_i124] = c1_iA[c1_i124];
  }

  *c1_b_nA = c1_nA;
  for (c1_i125 = 0; c1_i125 < 40; c1_i125++) {
    c1_b_iC[c1_i125] = c1_iC[c1_i125];
  }

  c1_b_DropConstraint(chartInstance, c1_kDrop, c1_b_iA, c1_b_nA, c1_b_iC);
}

static void c1_Unconstrained(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Hinv[400], real_T c1_f[20], real_T c1_x[20], int16_T
  c1_n, real_T c1_b_x[20])
{
  int32_T c1_i126;
  int32_T c1_i127;
  int32_T c1_i128;
  real_T c1_b_Hinv[400];
  real_T c1_b_f[20];
  for (c1_i126 = 0; c1_i126 < 20; c1_i126++) {
    c1_b_x[c1_i126] = c1_x[c1_i126];
  }

  for (c1_i127 = 0; c1_i127 < 400; c1_i127++) {
    c1_b_Hinv[c1_i127] = c1_Hinv[c1_i127];
  }

  for (c1_i128 = 0; c1_i128 < 20; c1_i128++) {
    c1_b_f[c1_i128] = c1_f[c1_i128];
  }

  c1_b_Unconstrained(chartInstance, c1_b_Hinv, c1_b_f, c1_b_x, c1_n);
}

static real_T c1_norm(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                      real_T c1_x[20])
{
  real_T c1_y;
  real_T c1_scale;
  int32_T c1_k;
  real_T c1_b_x;
  real_T c1_c_x;
  real_T c1_d_x;
  real_T c1_absxk;
  real_T c1_t;
  (void)chartInstance;
  c1_y = 0.0;
  c1_scale = 2.2250738585072014E-308;
  for (c1_k = 0; c1_k + 1 < 21; c1_k++) {
    c1_b_x = c1_x[c1_k];
    c1_c_x = c1_b_x;
    c1_d_x = c1_c_x;
    c1_absxk = muDoubleScalarAbs(c1_d_x);
    if (c1_absxk > c1_scale) {
      c1_t = c1_scale / c1_absxk;
      c1_y = 1.0 + c1_y * c1_t * c1_t;
      c1_scale = c1_absxk;
    } else {
      c1_t = c1_absxk / c1_scale;
      c1_y += c1_t * c1_t;
    }
  }

  return c1_scale * muDoubleScalarSqrt(c1_y);
}

static void c1_abs(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                   real_T c1_x[20], real_T c1_y[20])
{
  int32_T c1_k;
  real_T c1_b_x;
  real_T c1_b_y;
  (void)chartInstance;
  for (c1_k = 0; c1_k + 1 < 21; c1_k++) {
    c1_b_x = c1_x[c1_k];
    c1_b_y = muDoubleScalarAbs(c1_b_x);
    c1_y[c1_k] = c1_b_y;
  }
}

static void c1_b_abs(SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance,
                     real_T c1_x[40], real_T c1_y[40])
{
  int32_T c1_k;
  real_T c1_b_x;
  real_T c1_b_y;
  (void)chartInstance;
  for (c1_k = 0; c1_k + 1 < 41; c1_k++) {
    c1_b_x = c1_x[c1_k];
    c1_b_y = muDoubleScalarAbs(c1_b_x);
    c1_y[c1_k] = c1_b_y;
  }
}

static const mxArray *c1_l_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData;
  int32_T c1_b_u;
  const mxArray *c1_y = NULL;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_mxArrayOutData = NULL;
  c1_b_u = *(int32_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_b_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, false);
  return c1_mxArrayOutData;
}

static int32_T c1_j_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  int32_T c1_y;
  int32_T c1_i129;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), &c1_i129, 1, 6, 0U, 0, 0U, 0);
  c1_y = c1_i129;
  sf_mex_destroy(&c1_b_u);
  return c1_y;
}

static void c1_i_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_sfEvent;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  int32_T c1_y;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_sfEvent = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_y = c1_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_sfEvent),
    &c1_thisId);
  sf_mex_destroy(&c1_b_sfEvent);
  *(int32_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static void c1_k_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[400])
{
  real_T c1_dv14[400];
  int32_T c1_i130;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_dv14, 1, 0, 0U, 1, 0U, 2, 20,
                20);
  for (c1_i130 = 0; c1_i130 < 400; c1_i130++) {
    c1_y[c1_i130] = c1_dv14[c1_i130];
  }

  sf_mex_destroy(&c1_b_u);
}

static void c1_j_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_Linv;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[400];
  int32_T c1_i131;
  int32_T c1_i132;
  int32_T c1_i133;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_Linv = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_k_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_Linv), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_b_Linv);
  c1_i131 = 0;
  for (c1_i132 = 0; c1_i132 < 20; c1_i132++) {
    for (c1_i133 = 0; c1_i133 < 20; c1_i133++) {
      (*(real_T (*)[400])c1_outData)[c1_i133 + c1_i131] = c1_y[c1_i133 + c1_i131];
    }

    c1_i131 += 20;
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static c1_scZlGNcmSdXsfgNftx2nQBF c1_l_emlrt_marshallIn
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, const mxArray
   *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  c1_scZlGNcmSdXsfgNftx2nQBF c1_y;
  emlrtMsgIdentifier c1_thisId;
  static const char * c1_fieldNames[4] = { "DataType", "MaxIter",
    "FeasibilityTol", "IntegrityChecks" };

  c1_thisId.fParent = c1_parentId;
  c1_thisId.bParentIsCell = false;
  sf_mex_check_struct(c1_parentId, c1_b_u, 4, c1_fieldNames, 0U, NULL);
  c1_thisId.fIdentifier = "DataType";
  c1_m_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield(c1_b_u,
    "DataType", "DataType", 0)), &c1_thisId, c1_y.DataType);
  c1_thisId.fIdentifier = "MaxIter";
  c1_y.MaxIter = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getfield
    (c1_b_u, "MaxIter", "MaxIter", 0)), &c1_thisId);
  c1_thisId.fIdentifier = "FeasibilityTol";
  c1_y.FeasibilityTol = c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c1_b_u, "FeasibilityTol", "FeasibilityTol", 0)), &c1_thisId);
  c1_thisId.fIdentifier = "IntegrityChecks";
  c1_y.IntegrityChecks = c1_n_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getfield(c1_b_u, "IntegrityChecks", "IntegrityChecks", 0)),
    &c1_thisId);
  sf_mex_destroy(&c1_b_u);
  return c1_y;
}

static void c1_m_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId,
  char_T c1_y[6])
{
  char_T c1_cv9[6];
  int32_T c1_i134;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), c1_cv9, 1, 10, 0U, 1, 0U, 2, 1,
                6);
  for (c1_i134 = 0; c1_i134 < 6; c1_i134++) {
    c1_y[c1_i134] = c1_cv9[c1_i134];
  }

  sf_mex_destroy(&c1_b_u);
}

static boolean_T c1_n_emlrt_marshallIn
  (SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance, const mxArray
   *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  boolean_T c1_y;
  boolean_T c1_b1;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), &c1_b1, 1, 11, 0U, 0, 0U, 0);
  c1_y = c1_b1;
  sf_mex_destroy(&c1_b_u);
  return c1_y;
}

static void c1_k_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_opt;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  c1_scZlGNcmSdXsfgNftx2nQBF c1_y;
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)
    chartInstanceVoid;
  c1_b_opt = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_y = c1_l_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_opt), &c1_thisId);
  sf_mex_destroy(&c1_b_opt);
  *(c1_scZlGNcmSdXsfgNftx2nQBF *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static uint8_T c1_o_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance, const mxArray *c1_b_is_active_c1_DoD_simulink_model_2016, const
  char_T *c1_identifier)
{
  uint8_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_thisId.bParentIsCell = false;
  c1_y = c1_p_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c1_b_is_active_c1_DoD_simulink_model_2016), &c1_thisId);
  sf_mex_destroy(&c1_b_is_active_c1_DoD_simulink_model_2016);
  return c1_y;
}

static uint8_T c1_p_emlrt_marshallIn(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance, const mxArray *c1_b_u, const emlrtMsgIdentifier *c1_parentId)
{
  uint8_T c1_y;
  uint8_T c1_b_u0;
  (void)chartInstance;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_b_u), &c1_b_u0, 1, 3, 0U, 0, 0U, 0);
  c1_y = c1_b_u0;
  sf_mex_destroy(&c1_b_u);
  return c1_y;
}

static real_T c1_b_KWIKfactor(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Ac[800], int16_T c1_iC[40], int16_T c1_nA, real_T
  c1_RLinv[400], real_T c1_D[400], real_T c1_H[400])
{
  real_T c1_Status;
  int32_T c1_i135;
  int16_T c1_b_b;
  int16_T c1_c_b;
  boolean_T c1_overflow;
  int16_T c1_i;
  int32_T c1_i136;
  int32_T c1_b_iC;
  int32_T c1_i137;
  real_T c1_b_RLinv[400];
  real_T c1_QQ[400];
  real_T c1_RR[400];
  int16_T c1_d_b;
  int32_T c1_b_i;
  real_T c1_e_b[20];
  int16_T c1_f_b;
  int32_T c1_i138;
  boolean_T c1_b_overflow;
  int16_T c1_c_i;
  int32_T c1_d_i;
  int32_T c1_i139;
  int16_T c1_e_i;
  real_T c1_x;
  real_T c1_b_x;
  real_T c1_c_x;
  int32_T c1_i140;
  int32_T c1_i141;
  int16_T c1_j;
  real_T c1_y;
  static real_T c1_b_a[400] = { 0.17953118100446194, -0.35596589217268981,
    -0.31212981554100422, -0.22675653524786851, -0.13836709348478629,
    -0.065390865873951043, -0.017153889967133736, 0.0078411135547972188,
    0.016792892042678826, 0.017113889112630693, 0.01404679602414171,
    0.010482631107179853, 0.007609144225287896, 0.0056503763174164147,
    0.0044191163595166541, 0.0036378843765188257, 0.0030806749308000141,
    0.0026078701975466852, 0.0021524874181201204, 0.0026191208278017273, 0.0,
    0.42314360962586206, -0.15085880017441863, -0.24895431198843393,
    -0.21205994187589813, -0.1357940312684259, -0.065402106435703475,
    -0.017973592955685527, 0.006754449033805573, 0.015731139145787214,
    0.016185625920936923, 0.013258331794745552, 0.0098036329352445932,
    0.0070043049750641261, 0.0050913460341440427, 0.0038871871558543722,
    0.0031226486759438109, 0.0025774372668732233, 0.0021152123431113232,
    0.0026019082051462069, 0.0, 0.0, 0.5395265851382075, -0.014262270707080226,
    -0.18109207087873616, -0.18485281862460215, -0.12923880337950336,
    -0.0688067570398097, -0.025014422905191645, -0.00047862591763447643,
    0.0096940723600378014, 0.011566364650929865, 0.0097942556978274555,
    0.0071348572763342095, 0.0048392200597921981, 0.0032452913254430115,
    0.0022647328163359392, 0.0016850269629045736, 0.0013143364410911163,
    0.0016216283611085358, 0.0, 0.0, 0.0, 0.59211926551196981,
    0.047328381414249318, -0.15268622922557878, -0.17729389107872212,
    -0.13199197751842245, -0.075511091762731422, -0.032129433380572163,
    -0.0064963725742828932, 0.0050730630574488929, 0.0081158154215212278,
    0.0071668280151789554, 0.0050399411987726694, 0.0030882386581324041,
    0.0017374311523575998, 0.00095607535964168831, 0.00056105746294453937,
    0.00059898831032237419, 0.0, 0.0, 0.0, 0.0, 0.61112340659458753,
    0.066428855805202885, -0.14633752595678243, -0.1781025731904336,
    -0.13587450839489759, -0.080056532210652731, -0.036169443736289414,
    -0.0096980143063973984, 0.0026362238008596039, 0.0062504460102224026,
    0.0056942409780984986, 0.003836387614978537, 0.0020818899277130921,
    0.0008919675437983743, 0.00025484061500147552, -4.9249698557432218E-5, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.61609645345964925, 0.070789842264748715,
    -0.14512159360004248, -0.17874273757436696, -0.13739387415204857,
    -0.081770412971494066, -0.037737852126643358, -0.011019124823261331,
    0.0015581893404897267, 0.0053808926434729936, 0.0049980320983291594,
    0.0032855725395688645, 0.001655529200958293, 0.00057437073779722082,
    -0.00015152176150694629, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.61752066298854769,
    0.073139856854092244, -0.14313191294784083, -0.17757747081585479,
    -0.13698087464975267, -0.081855393455244174, -0.038094337555445383,
    -0.011482825060102213, 0.001096673102422268, 0.0049859139481912883,
    0.004699451064870676, 0.0030899366817990831, 0.0015564764276950239,
    0.000383387430390087, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.618877712158251,
    0.076332096826689511, -0.13998345115540026, -0.1753057517130274,
    -0.13566229424971757, -0.081286692035685393, -0.038015764072695642,
    -0.011662981367827761, 0.00082945324616624837, 0.0047397989263093642,
    0.0045303506316400708, 0.0030184046344605936, 0.0015945134104339185, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.620588804519038, 0.080166901496609044,
    -0.13624480592327431, -0.17253210532067745, -0.13397358139977011,
    -0.080492196616852987, -0.037831682769763404, -0.011820631184603076,
    0.000536667336363688, 0.0044473630990359064, 0.00431418933685784,
    0.00333741516032808, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.62248419151390333, 0.084335946763323524, -0.132123897204966,
    -0.16940390222489501, -0.13203533829528402, -0.079581195629996465,
    -0.037652580568818823, -0.012075207685536573, 8.8255026121634059E-5,
    0.0039706503037129281, 0.004983569800658629, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.62451257844946806, 0.088881192432794964,
    -0.12750262621125666, -0.16581111357097594, -0.12977631038826129,
    -0.078530614410585509, -0.037505507434398136, -0.012501775660427579,
    -0.00063570557752505607, 0.0049863787430134976, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.626749742358861, 0.094031612726143993,
    -0.12211944601290357, -0.16152819643017485, -0.12703444898697747,
    -0.077255324545548953, -0.037390146457159851, -0.013192755931829855,
    0.00047065099914092822, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.629321414091852, 0.10009345772427337, -0.11562337865774583,
    -0.15623097718874451, -0.1235574443351526, -0.0756096624066169,
    -0.037301806748486946, -0.012768012332613427, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.63238080360261406, 0.10745698452253324,
    -0.10752667340175709, -0.14943043251889698, -0.11893450275079041,
    -0.073338786154699, -0.039135056992419641, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.63612782549517355, 0.11667325035042805,
    -0.097082020512002751, -0.14031711491355309, -0.11242510917588491,
    -0.079928086371240215, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.64085382460804885, 0.12859974089490123,
    -0.083036421923058462, -0.12741027099252486, -0.12674359755585715, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.64702223690479121, 0.14468716622648875, -0.063080140974317231,
    -0.15127058736482824, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6554317041768738, 0.16762711643300396,
    -0.093565438726647021, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.66759498812525553, 0.14412109784377947,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.68298139053534912 };

  int16_T c1_b_j;
  int32_T c1_f_i;
  int32_T c1_i142;
  int16_T c1_g_i;
  int16_T c1_c_a;
  int32_T c1_c_j;
  real_T c1_d_a[20];
  int16_T c1_g_b;
  int32_T c1_i143;
  int16_T c1_h_b;
  int16_T c1_d_j;
  int16_T c1_e_a;
  int16_T c1_i_b;
  int16_T c1_j_b;
  boolean_T c1_c_overflow;
  boolean_T c1_d_overflow;
  real_T c1_b_y;
  int32_T c1_k;
  int32_T c1_i144;
  int16_T c1_e_j;
  int16_T c1_b_k;
  real_T c1_TL[400];
  int16_T c1_h_i;
  int16_T c1_i145;
  real_T c1_A;
  int16_T c1_c_k;
  int32_T c1_i146;
  real_T c1_B;
  real_T c1_d_x;
  real_T c1_c_y;
  int16_T c1_f_a;
  real_T c1_e_x;
  int16_T c1_k_b;
  int16_T c1_i147;
  real_T c1_d_y;
  int16_T c1_g_a;
  int16_T c1_l_b;
  real_T c1_e_y;
  int16_T c1_m_b;
  int16_T c1_n_b;
  boolean_T c1_e_overflow;
  boolean_T c1_f_overflow;
  int16_T c1_d_k;
  int16_T c1_i_i;
  int16_T c1_h_a;
  int16_T c1_o_b;
  int16_T c1_i_a;
  int16_T c1_p_b;
  boolean_T c1_g_overflow;
  int16_T c1_e_k;
  int32_T exitg1;
  c1_Status = 1.0;
  for (c1_i135 = 0; c1_i135 < 400; c1_i135++) {
    c1_RLinv[c1_i135] = 0.0;
  }

  c1_b_b = c1_nA;
  c1_c_b = c1_b_b;
  c1_overflow = ((!(1 > c1_c_b)) && (c1_c_b > 32766));
  if (c1_overflow) {
    c1_b_check_forloop_overflow_error(chartInstance, true);
  }

  for (c1_i = 1; c1_i <= c1_nA; c1_i++) {
    c1_b_iC = (int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
      chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_iC[(int16_T)
      sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct, chartInstance->S,
      1U, 0, 0, MAX_uint32_T, (int32_T)c1_i, 1, 40) - 1], 1, 40) - 1;
    for (c1_i137 = 0; c1_i137 < 20; c1_i137++) {
      c1_e_b[c1_i137] = c1_Ac[c1_b_iC + 40 * c1_i137];
    }

    c1_b_i = (int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
      chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_i, 1, 20) - 1;
    for (c1_i138 = 0; c1_i138 < 20; c1_i138++) {
      c1_RLinv[c1_i138 + 20 * c1_b_i] = 0.0;
    }

    (int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
      chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_i, 1, 20);
    c1_d_i = (int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
      chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_i, 1, 20) - 1;
    for (c1_i139 = 0; c1_i139 < 20; c1_i139++) {
      c1_RLinv[c1_i139 + 20 * c1_d_i] = 0.0;
      for (c1_i140 = 0; c1_i140 < 20; c1_i140++) {
        c1_RLinv[c1_i139 + 20 * c1_d_i] += c1_b_a[c1_i139 + 20 * c1_i140] *
          c1_e_b[c1_i140];
      }
    }
  }

  for (c1_i136 = 0; c1_i136 < 400; c1_i136++) {
    c1_b_RLinv[c1_i136] = c1_RLinv[c1_i136];
  }

  c1_qr(chartInstance, c1_b_RLinv, c1_QQ, c1_RR);
  c1_d_b = c1_nA;
  c1_f_b = c1_d_b;
  c1_b_overflow = ((!(1 > c1_f_b)) && (c1_f_b > 32766));
  if (c1_b_overflow) {
    c1_b_check_forloop_overflow_error(chartInstance, true);
  }

  c1_c_i = 1;
  do {
    exitg1 = 0;
    if (c1_c_i <= c1_nA) {
      c1_x = c1_RR[((int16_T)sf_eml_array_bounds_check
                    (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                     MAX_uint32_T, (int32_T)c1_c_i, 1, 20) + 20 * ((int16_T)
        sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct, chartInstance->S,
        1U, 0, 0, MAX_uint32_T, (int32_T)c1_c_i, 1, 20) - 1)) - 1];
      c1_b_x = c1_x;
      c1_c_x = c1_b_x;
      c1_y = muDoubleScalarAbs(c1_c_x);
      if (c1_y < 1.0E-12) {
        c1_Status = -2.0;
        exitg1 = 1;
      } else {
        c1_c_i++;
      }
    } else {
      for (c1_e_i = 1; c1_e_i < 21; c1_e_i++) {
        for (c1_j = 1; c1_j < 21; c1_j++) {
          c1_f_i = c1_e_i - 1;
          for (c1_i142 = 0; c1_i142 < 20; c1_i142++) {
            c1_d_a[c1_i142] = c1_b_a[c1_i142 + 20 * c1_f_i];
          }

          c1_c_j = c1_j - 1;
          for (c1_i143 = 0; c1_i143 < 20; c1_i143++) {
            c1_e_b[c1_i143] = c1_QQ[c1_i143 + 20 * c1_c_j];
          }

          c1_b_y = 0.0;
          for (c1_k = 0; c1_k + 1 < 21; c1_k++) {
            c1_b_y += c1_d_a[c1_k] * c1_e_b[c1_k];
          }

          c1_TL[(c1_e_i + 20 * (c1_j - 1)) - 1] = c1_b_y;
        }
      }

      for (c1_i141 = 0; c1_i141 < 400; c1_i141++) {
        c1_RLinv[c1_i141] = 0.0;
      }

      for (c1_b_j = c1_nA; c1_b_j > 0; c1_b_j--) {
        c1_RLinv[((int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
                   chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_b_j, 1,
                   20) + 20 * ((int16_T)sf_eml_array_bounds_check
                   (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                    MAX_uint32_T, (int32_T)c1_b_j, 1, 20) - 1)) - 1] = 1.0;
        c1_c_a = c1_b_j;
        c1_g_b = c1_nA;
        c1_e_a = c1_c_a;
        c1_j_b = c1_g_b;
        c1_d_overflow = ((!(c1_e_a > c1_j_b)) && (c1_j_b > 32766));
        if (c1_d_overflow) {
          c1_b_check_forloop_overflow_error(chartInstance, true);
        }

        for (c1_b_k = c1_b_j; c1_b_k <= c1_nA; c1_b_k++) {
          c1_A = c1_RLinv[(c1_b_j + 20 * ((int16_T)sf_eml_array_bounds_check
            (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
             MAX_uint32_T, (int32_T)c1_b_k, 1, 20) - 1)) - 1];
          c1_B = c1_RR[(c1_b_j + 20 * (c1_b_j - 1)) - 1];
          c1_d_x = c1_A;
          c1_c_y = c1_B;
          c1_e_x = c1_d_x;
          c1_d_y = c1_c_y;
          c1_e_y = c1_e_x / c1_d_y;
          c1_RLinv[(c1_b_j + 20 * ((int16_T)sf_eml_array_bounds_check
                     (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                      MAX_uint32_T, (int32_T)c1_b_k, 1, 20) - 1)) - 1] = c1_e_y;
        }

        if (c1_b_j > 1) {
          c1_i146 = c1_b_j - 1;
          if (c1_i146 > 32767) {
            c1_i146 = 32767;
          } else {
            if (c1_i146 < -32768) {
              c1_i146 = -32768;
            }
          }

          c1_i147 = (int16_T)c1_i146;
          c1_l_b = c1_i147;
          c1_n_b = c1_l_b;
          c1_f_overflow = ((!(1 > c1_n_b)) && (c1_n_b > 32766));
          if (c1_f_overflow) {
            c1_b_check_forloop_overflow_error(chartInstance, true);
          }

          for (c1_i_i = 1; c1_i_i <= c1_i147; c1_i_i++) {
            c1_h_a = c1_b_j;
            c1_o_b = c1_nA;
            c1_i_a = c1_h_a;
            c1_p_b = c1_o_b;
            c1_g_overflow = ((!(c1_i_a > c1_p_b)) && (c1_p_b > 32766));
            if (c1_g_overflow) {
              c1_b_check_forloop_overflow_error(chartInstance, true);
            }

            for (c1_e_k = c1_b_j; c1_e_k <= c1_nA; c1_e_k++) {
              c1_RLinv[(c1_i_i + 20 * ((int16_T)sf_eml_array_bounds_check
                         (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0,
                          0, MAX_uint32_T, (int32_T)c1_e_k, 1, 20) - 1)) - 1] =
                c1_RLinv[(c1_i_i + 20 * ((int16_T)sf_eml_array_bounds_check
                (sfGlobalDebugInstanceStruct, chartInstance->S, 1U, 0, 0,
                 MAX_uint32_T, (int32_T)c1_e_k, 1, 20) - 1)) - 1] - c1_RR
                [(c1_i_i + 20 * (c1_b_j - 1)) - 1] * c1_RLinv[(c1_b_j + 20 *
                ((int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
                chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_e_k, 1, 20)
                 - 1)) - 1];
            }
          }
        }
      }

      for (c1_g_i = 1; c1_g_i < 21; c1_g_i++) {
        for (c1_d_j = c1_g_i; c1_d_j < 21; c1_d_j++) {
          c1_H[(c1_g_i + 20 * (c1_d_j - 1)) - 1] = 0.0;
          c1_i144 = c1_nA + 1;
          if (c1_i144 > 32767) {
            c1_i144 = 32767;
          } else {
            if (c1_i144 < -32768) {
              c1_i144 = -32768;
            }
          }

          c1_i145 = (int16_T)c1_i144;
          for (c1_c_k = c1_i145; c1_c_k < 21; c1_c_k++) {
            c1_H[(c1_g_i + 20 * (c1_d_j - 1)) - 1] -= c1_TL[(c1_g_i + 20 *
              ((int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
              chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_c_k, 1, 20)
               - 1)) - 1] * c1_TL[(c1_d_j + 20 * ((int16_T)
              sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
              chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_c_k, 1, 20)
              - 1)) - 1];
          }

          c1_H[(c1_d_j + 20 * (c1_g_i - 1)) - 1] = c1_H[(c1_g_i + 20 * (c1_d_j -
            1)) - 1];
        }
      }

      c1_h_b = c1_nA;
      c1_i_b = c1_h_b;
      c1_c_overflow = ((!(1 > c1_i_b)) && (c1_i_b > 32766));
      if (c1_c_overflow) {
        c1_b_check_forloop_overflow_error(chartInstance, true);
      }

      for (c1_e_j = 1; c1_e_j <= c1_nA; c1_e_j++) {
        for (c1_h_i = 1; c1_h_i < 21; c1_h_i++) {
          c1_D[(c1_h_i + 20 * ((int16_T)sf_eml_array_bounds_check
                               (sfGlobalDebugInstanceStruct, chartInstance->S,
                                1U, 0, 0, MAX_uint32_T, (int32_T)c1_e_j, 1, 20)
                               - 1)) - 1] = 0.0;
          c1_f_a = c1_e_j;
          c1_k_b = c1_nA;
          c1_g_a = c1_f_a;
          c1_m_b = c1_k_b;
          c1_e_overflow = ((!(c1_g_a > c1_m_b)) && (c1_m_b > 32766));
          if (c1_e_overflow) {
            c1_b_check_forloop_overflow_error(chartInstance, true);
          }

          for (c1_d_k = c1_e_j; c1_d_k <= c1_nA; c1_d_k++) {
            c1_D[(c1_h_i + 20 * (c1_e_j - 1)) - 1] += c1_TL[(c1_h_i + 20 *
              ((int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
              chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_d_k, 1, 20)
               - 1)) - 1] * c1_RLinv[(c1_e_j + 20 * ((int16_T)
              sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
              chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_d_k, 1, 20)
              - 1)) - 1];
          }
        }
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return c1_Status;
}

static void c1_b_xgeqrf(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_A[400], real_T c1_tau[20])
{
  ptrdiff_t c1_m_t;
  ptrdiff_t c1_n_t;
  ptrdiff_t c1_info_t;
  int32_T c1_info;
  int32_T c1_b_info;
  int32_T c1_c_info;
  boolean_T c1_p;
  boolean_T c1_b_p;
  int32_T c1_val;
  boolean_T c1_c_p;
  int32_T c1_i148;
  int32_T c1_i149;
  c1_m_t = (ptrdiff_t)20;
  c1_n_t = (ptrdiff_t)20;
  c1_info_t = LAPACKE_dgeqrf(102, c1_m_t, c1_n_t, &c1_A[0], c1_m_t, &c1_tau[0]);
  c1_info = (int32_T)c1_info_t;
  c1_b_info = c1_info;
  c1_c_info = c1_b_info;
  c1_p = (c1_c_info != 0);
  if (c1_p) {
    c1_b_p = true;
    c1_val = c1_b_info;
    c1_c_p = false;
    if (c1_val == -4) {
      c1_c_p = true;
    }

    if (!c1_c_p) {
      if (c1_b_info == -1010) {
        c1_error(chartInstance);
      } else {
        c1_b_error(chartInstance, c1_b_info);
      }
    }
  } else {
    c1_b_p = false;
  }

  if (c1_b_p) {
    for (c1_i148 = 0; c1_i148 < 400; c1_i148++) {
      c1_A[c1_i148] = rtNaN;
    }

    for (c1_i149 = 0; c1_i149 < 20; c1_i149++) {
      c1_tau[c1_i149] = rtNaN;
    }
  }
}

static void c1_b_DropConstraint(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, int16_T c1_kDrop, int16_T c1_iA[40], int16_T *c1_nA, int16_T
  c1_iC[40])
{
  int32_T c1_i150;
  int32_T c1_i151;
  int16_T c1_i152;
  int16_T c1_b_a;
  int16_T c1_b_b;
  int16_T c1_c_a;
  int16_T c1_c_b;
  boolean_T c1_overflow;
  int16_T c1_i;
  int32_T c1_i153;
  c1_iA[(int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
    chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_iC[(int16_T)
    sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct, chartInstance->S, 1U,
    0, 0, MAX_uint32_T, (int32_T)c1_kDrop, 1, 40) - 1], 1, 40) - 1] = 0;
  if (c1_kDrop < *c1_nA) {
    c1_i150 = *c1_nA - 1;
    if (c1_i150 > 32767) {
      c1_i150 = 32767;
    } else {
      if (c1_i150 < -32768) {
        c1_i150 = -32768;
      }
    }

    c1_i152 = (int16_T)c1_i150;
    c1_b_a = c1_kDrop;
    c1_b_b = c1_i152;
    c1_c_a = c1_b_a;
    c1_c_b = c1_b_b;
    c1_overflow = ((!(c1_c_a > c1_c_b)) && (c1_c_b > 32766));
    if (c1_overflow) {
      c1_b_check_forloop_overflow_error(chartInstance, true);
    }

    for (c1_i = c1_kDrop; c1_i <= c1_i152; c1_i++) {
      c1_i153 = c1_i + 1;
      if (c1_i153 > 32767) {
        c1_i153 = 32767;
      } else {
        if (c1_i153 < -32768) {
          c1_i153 = -32768;
        }
      }

      c1_iC[(int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
        chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)c1_i, 1, 40) - 1] =
        c1_iC[(int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
        chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)(int16_T)c1_i153, 1,
        40) - 1];
    }
  }

  c1_iC[(int16_T)sf_eml_array_bounds_check(sfGlobalDebugInstanceStruct,
    chartInstance->S, 1U, 0, 0, MAX_uint32_T, (int32_T)*c1_nA, 1, 40) - 1] = 0;
  c1_i151 = *c1_nA - 1;
  if (c1_i151 > 32767) {
    c1_i151 = 32767;
  } else {
    if (c1_i151 < -32768) {
      c1_i151 = -32768;
    }
  }

  *c1_nA = (int16_T)c1_i151;
}

static void c1_b_Unconstrained(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance, real_T c1_Hinv[400], real_T c1_f[20], real_T c1_x[20], int16_T
  c1_n)
{
  int16_T c1_i;
  int32_T c1_b_i;
  int32_T c1_i154;
  real_T c1_y;
  real_T c1_b_a[20];
  int32_T c1_k;
  (void)chartInstance;
  (void)c1_n;
  for (c1_i = 1; c1_i < 21; c1_i++) {
    c1_b_i = c1_i - 1;
    for (c1_i154 = 0; c1_i154 < 20; c1_i154++) {
      c1_b_a[c1_i154] = -c1_Hinv[c1_b_i + 20 * c1_i154];
    }

    c1_y = 0.0;
    for (c1_k = 0; c1_k + 1 < 21; c1_k++) {
      c1_y += c1_b_a[c1_k] * c1_f[c1_k];
    }

    c1_x[c1_i - 1] = c1_y;
  }
}

static void init_dsm_address_info(SFc1_DoD_simulink_model_2016InstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_simulink_io_address(SFc1_DoD_simulink_model_2016InstanceStruct *
  chartInstance)
{
  chartInstance->c1_ref = (real_T *)ssGetInputPortSignal_wrapper
    (chartInstance->S, 0);
  chartInstance->c1_u = (real_T *)ssGetOutputPortSignal_wrapper(chartInstance->S,
    1);
  chartInstance->c1_mu0 = (real_T (*)[3])ssGetInputPortSignal_wrapper
    (chartInstance->S, 1);
  chartInstance->c1_u0 = (real_T *)ssGetInputPortSignal_wrapper(chartInstance->S,
    2);
  chartInstance->c1_status = (real_T *)ssGetOutputPortSignal_wrapper
    (chartInstance->S, 2);
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c1_DoD_simulink_model_2016_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(2263875446U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(421513842U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2434055022U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1377838942U);
}

mxArray* sf_c1_DoD_simulink_model_2016_get_post_codegen_info(void);
mxArray *sf_c1_DoD_simulink_model_2016_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals", "postCodegenInfo" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1, 1, sizeof
    (autoinheritanceFields)/sizeof(autoinheritanceFields[0]),
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("n33pMwakxOr5UBcHdsZgUB");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,3,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,8,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      pr[1] = (double)(3);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(40);
      pr[1] = (double)(20);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(3);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(20);
      pr[1] = (double)(5);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(20);
      pr[1] = (double)(20);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,1,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(40);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,6,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,6,"type",mxType);
    }

    mxSetField(mxData,6,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,7,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(13));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,7,"type",mxType);
    }

    mxSetField(mxData,7,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxData);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,2,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,0,mxREAL);
      double *pr = mxGetPr(mxSize);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt", "isFixedPointType" };

      mxArray *mxType = mxCreateStructMatrix(1,1,sizeof(typeFields)/sizeof
        (typeFields[0]),typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxType,0,"isFixedPointType",mxCreateDoubleScalar(0));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  {
    mxArray* mxPostCodegenInfo =
      sf_c1_DoD_simulink_model_2016_get_post_codegen_info();
    mxSetField(mxAutoinheritanceInfo,0,"postCodegenInfo",mxPostCodegenInfo);
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c1_DoD_simulink_model_2016_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,2);
  mxSetCell(mxcell3p, 0, mxCreateString("coder.internal.blas.BLASApi"));
  mxSetCell(mxcell3p, 1, mxCreateString("coder.internal.lapack.LAPACKApi"));
  return(mxcell3p);
}

mxArray *sf_c1_DoD_simulink_model_2016_jit_fallback_info(void)
{
  const char *infoFields[] = { "fallbackType", "fallbackReason",
    "hiddenFallbackType", "hiddenFallbackReason", "incompatibleSymbol" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 5, infoFields);
  mxArray *fallbackType = mxCreateString("pre");
  mxArray *fallbackReason = mxCreateString("hasBreakpoints");
  mxArray *hiddenFallbackType = mxCreateString("none");
  mxArray *hiddenFallbackReason = mxCreateString("");
  mxArray *incompatibleSymbol = mxCreateString("");
  mxSetField(mxInfo, 0, infoFields[0], fallbackType);
  mxSetField(mxInfo, 0, infoFields[1], fallbackReason);
  mxSetField(mxInfo, 0, infoFields[2], hiddenFallbackType);
  mxSetField(mxInfo, 0, infoFields[3], hiddenFallbackReason);
  mxSetField(mxInfo, 0, infoFields[4], incompatibleSymbol);
  return mxInfo;
}

mxArray *sf_c1_DoD_simulink_model_2016_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

mxArray* sf_c1_DoD_simulink_model_2016_get_post_codegen_info(void)
{
  const char* fieldNames[] = { "exportedFunctionsUsedByThisChart",
    "exportedFunctionsChecksum" };

  mwSize dims[2] = { 1, 1 };

  mxArray* mxPostCodegenInfo = mxCreateStructArray(2, dims, sizeof(fieldNames)/
    sizeof(fieldNames[0]), fieldNames);

  {
    mxArray* mxExportedFunctionsChecksum = mxCreateString("");
    mwSize exp_dims[2] = { 0, 1 };

    mxArray* mxExportedFunctionsUsedByThisChart = mxCreateCellArray(2, exp_dims);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsUsedByThisChart",
               mxExportedFunctionsUsedByThisChart);
    mxSetField(mxPostCodegenInfo, 0, "exportedFunctionsChecksum",
               mxExportedFunctionsChecksum);
  }

  return mxPostCodegenInfo;
}

static const mxArray *sf_get_sim_state_info_c1_DoD_simulink_model_2016(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x3'type','srcId','name','auxInfo'{{M[1],M[5],T\"status\",},{M[1],M[4],T\"u\",},{M[8],M[0],T\"is_active_c1_DoD_simulink_model_2016\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 3, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c1_DoD_simulink_model_2016_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance =
      (SFc1_DoD_simulink_model_2016InstanceStruct *)sf_get_chart_instance_ptr(S);
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _DoD_simulink_model_2016MachineNumber_,
           1,
           1,
           1,
           0,
           13,
           0,
           0,
           0,
           0,
           0,
           &chartInstance->chartNumber,
           &chartInstance->instanceNumber,
           (void *)S);

        /* Each instance must initialize its own list of scripts */
        init_script_number_translation(_DoD_simulink_model_2016MachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_DoD_simulink_model_2016MachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _DoD_simulink_model_2016MachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"ref");
          _SFD_SET_DATA_PROPS(1,1,1,0,"mu0");
          _SFD_SET_DATA_PROPS(2,1,1,0,"u0");
          _SFD_SET_DATA_PROPS(3,2,0,1,"u");
          _SFD_SET_DATA_PROPS(4,2,0,1,"status");
          _SFD_SET_DATA_PROPS(5,10,0,0,"Acd");
          _SFD_SET_DATA_PROPS(6,10,0,0,"Aineq");
          _SFD_SET_DATA_PROPS(7,10,0,0,"Bcd");
          _SFD_SET_DATA_PROPS(8,10,0,0,"Gamma");
          _SFD_SET_DATA_PROPS(9,10,0,0,"Linv");
          _SFD_SET_DATA_PROPS(10,10,0,0,"bineq");
          _SFD_SET_DATA_PROPS(11,10,0,0,"n_in");
          _SFD_SET_DATA_PROPS(12,10,0,0,"opt");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,0,0,0,0,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,536);
        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[1];
          dimVector[0]= 3U;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);

        {
          unsigned int dimVector[2];
          dimVector[0]= 3U;
          dimVector[1]= 3U;
          _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_e_sf_marshallOut,(MexInFcnForType)
            c1_e_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 40U;
          dimVector[1]= 20U;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_c_sf_marshallOut,(MexInFcnForType)
            c1_c_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 3U;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_d_sf_marshallOut,(MexInFcnForType)
            c1_d_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 20U;
          dimVector[1]= 5U;
          _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_f_sf_marshallOut,(MexInFcnForType)
            c1_f_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 20U;
          dimVector[1]= 20U;
          _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_h_sf_marshallOut,(MexInFcnForType)
            c1_j_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 40U;
          _SFD_SET_DATA_COMPILED_PROPS(10,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)
            c1_b_sf_marshallIn);
        }

        _SFD_SET_DATA_COMPILED_PROPS(11,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)c1_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(12,SF_STRUCT,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_g_sf_marshallOut,(MexInFcnForType)c1_k_sf_marshallIn);
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _DoD_simulink_model_2016MachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static void chart_debug_initialize_data_addresses(SimStruct *S)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance =
      (SFc1_DoD_simulink_model_2016InstanceStruct *)sf_get_chart_instance_ptr(S);
    if (ssIsFirstInitCond(S)) {
      /* do this only if simulation is starting and after we know the addresses of all data */
      {
        _SFD_SET_DATA_VALUE_PTR(0U, chartInstance->c1_ref);
        _SFD_SET_DATA_VALUE_PTR(3U, chartInstance->c1_u);
        _SFD_SET_DATA_VALUE_PTR(1U, *chartInstance->c1_mu0);
        _SFD_SET_DATA_VALUE_PTR(9U, chartInstance->c1_Linv);
        _SFD_SET_DATA_VALUE_PTR(12U, &chartInstance->c1_opt);
        _SFD_SET_DATA_VALUE_PTR(11U, &chartInstance->c1_n_in);
        _SFD_SET_DATA_VALUE_PTR(2U, chartInstance->c1_u0);
        _SFD_SET_DATA_VALUE_PTR(4U, chartInstance->c1_status);
        _SFD_SET_DATA_VALUE_PTR(8U, chartInstance->c1_Gamma);
        _SFD_SET_DATA_VALUE_PTR(5U, chartInstance->c1_Acd);
        _SFD_SET_DATA_VALUE_PTR(7U, chartInstance->c1_Bcd);
        _SFD_SET_DATA_VALUE_PTR(6U, chartInstance->c1_Aineq);
        _SFD_SET_DATA_VALUE_PTR(10U, chartInstance->c1_bineq);
      }
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "si39VBE7RSgapZZAKsG1TrB";
}

static void sf_opaque_initialize_c1_DoD_simulink_model_2016(void
  *chartInstanceVar)
{
  chart_debug_initialization(((SFc1_DoD_simulink_model_2016InstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c1_DoD_simulink_model_2016
    ((SFc1_DoD_simulink_model_2016InstanceStruct*) chartInstanceVar);
  initialize_c1_DoD_simulink_model_2016
    ((SFc1_DoD_simulink_model_2016InstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c1_DoD_simulink_model_2016(void *chartInstanceVar)
{
  enable_c1_DoD_simulink_model_2016((SFc1_DoD_simulink_model_2016InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c1_DoD_simulink_model_2016(void *chartInstanceVar)
{
  disable_c1_DoD_simulink_model_2016((SFc1_DoD_simulink_model_2016InstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c1_DoD_simulink_model_2016(void *chartInstanceVar)
{
  sf_gateway_c1_DoD_simulink_model_2016
    ((SFc1_DoD_simulink_model_2016InstanceStruct*) chartInstanceVar);
}

static const mxArray* sf_opaque_get_sim_state_c1_DoD_simulink_model_2016
  (SimStruct* S)
{
  return get_sim_state_c1_DoD_simulink_model_2016
    ((SFc1_DoD_simulink_model_2016InstanceStruct *)sf_get_chart_instance_ptr(S));/* raw sim ctx */
}

static void sf_opaque_set_sim_state_c1_DoD_simulink_model_2016(SimStruct* S,
  const mxArray *st)
{
  set_sim_state_c1_DoD_simulink_model_2016
    ((SFc1_DoD_simulink_model_2016InstanceStruct*)sf_get_chart_instance_ptr(S),
     st);
}

static void sf_opaque_terminate_c1_DoD_simulink_model_2016(void
  *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc1_DoD_simulink_model_2016InstanceStruct*)
                    chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_DoD_simulink_model_2016_optimization_info();
    }

    finalize_c1_DoD_simulink_model_2016
      ((SFc1_DoD_simulink_model_2016InstanceStruct*) chartInstanceVar);
    utFree(chartInstanceVar);
    if (ssGetUserData(S)!= NULL) {
      sf_free_ChartRunTimeInfo(S);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc1_DoD_simulink_model_2016
    ((SFc1_DoD_simulink_model_2016InstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c1_DoD_simulink_model_2016(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c1_DoD_simulink_model_2016
      ((SFc1_DoD_simulink_model_2016InstanceStruct*)sf_get_chart_instance_ptr(S));
  }
}

static void mdlSetWorkWidths_c1_DoD_simulink_model_2016(SimStruct *S)
{
  /* Actual parameters from chart:
     Acd Aineq Bcd Gamma Linv bineq n_in opt
   */
  const char_T *rtParamNames[] = { "Acd", "Aineq", "Bcd", "Gamma", "Linv",
    "bineq", "n_in", "opt" };

  ssSetNumRunTimeParams(S,ssGetSFcnParamsCount(S));

  /* registration for Acd*/
  ssRegDlgParamAsRunTimeParam(S, 0, 0, rtParamNames[0], SS_DOUBLE);

  /* registration for Aineq*/
  ssRegDlgParamAsRunTimeParam(S, 1, 1, rtParamNames[1], SS_DOUBLE);

  /* registration for Bcd*/
  ssRegDlgParamAsRunTimeParam(S, 2, 2, rtParamNames[2], SS_DOUBLE);

  /* registration for Gamma*/
  ssRegDlgParamAsRunTimeParam(S, 3, 3, rtParamNames[3], SS_DOUBLE);

  /* registration for Linv*/
  ssRegDlgParamAsRunTimeParam(S, 4, 4, rtParamNames[4], SS_DOUBLE);

  /* registration for bineq*/
  ssRegDlgParamAsRunTimeParam(S, 5, 5, rtParamNames[5], SS_DOUBLE);

  /* registration for n_in*/
  ssRegDlgParamAsRunTimeParam(S, 6, 6, rtParamNames[6], SS_DOUBLE);

  /* Set overwritable ports for inplace optimization */
  ssSetStatesModifiedOnlyInUpdate(S, 1);
  ssMdlUpdateIsEmpty(S, 1);
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_DoD_simulink_model_2016_optimization_info
      (sim_mode_is_rtw_gen(S), sim_mode_is_modelref_sim(S), sim_mode_is_external
       (S));
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,1);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,1);
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,1,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_set_chart_accesses_machine_info(S, sf_get_instance_specialization(),
      infoStruct, 1);
    sf_update_buildInfo(S, sf_get_instance_specialization(),infoStruct,1);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,1,3);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,1,2);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=2; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 3; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,1);
    sf_register_codegen_names_for_scoped_functions_defined_by_chart(S);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(3348279379U));
  ssSetChecksum1(S,(2991966927U));
  ssSetChecksum2(S,(1749326054U));
  ssSetChecksum3(S,(1800159827U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSetStateSemanticsClassicAndSynchronous(S, true);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c1_DoD_simulink_model_2016(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c1_DoD_simulink_model_2016(SimStruct *S)
{
  SFc1_DoD_simulink_model_2016InstanceStruct *chartInstance;
  chartInstance = (SFc1_DoD_simulink_model_2016InstanceStruct *)utMalloc(sizeof
    (SFc1_DoD_simulink_model_2016InstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  memset(chartInstance, 0, sizeof(SFc1_DoD_simulink_model_2016InstanceStruct));
  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.enableChart =
    sf_opaque_enable_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.mdlStart = mdlStart_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c1_DoD_simulink_model_2016;
  chartInstance->chartInfo.callGetHoverDataForMsg = NULL;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.callAtomicSubchartUserFcn = NULL;
  chartInstance->chartInfo.callAtomicSubchartAutoFcn = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  sf_init_ChartRunTimeInfo(S, &(chartInstance->chartInfo), false, 0);
  init_dsm_address_info(chartInstance);
  init_simulink_io_address(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  chart_debug_initialization(S,1);
  mdl_start_c1_DoD_simulink_model_2016(chartInstance);
}

void c1_DoD_simulink_model_2016_method_dispatcher(SimStruct *S, int_T method,
  void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c1_DoD_simulink_model_2016(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c1_DoD_simulink_model_2016(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c1_DoD_simulink_model_2016(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c1_DoD_simulink_model_2016_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
