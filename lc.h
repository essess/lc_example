/**
 * @brief   interface to local controller
 * @author  sstasiak@protonmail.com
 *
 * Copyright (c) 2018,2019 Sean Stasiak. All rights reserved.
 * Developed by: Sean Stasiak <sstasiak@protonmail.com>
 * Refer to license terms in license.txt; In the absence of such a file,
 * contact me at the above email address and I can provide you with one.
 *
 *
 *
 *              >> THIS LIBRARY IS NOT THREADSAFE
 *              >> ALL CALLS ARE BLOCKING
 *
 *
 */

#ifndef   _lc_h_
#define   _lc_h_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @public error results
 */
typedef enum
{
  ERR_NONE = 0,

  ERR_BADREF,
  ERR_BADHND,
  ERR_BADTEMP,

  ERR_UNKNOWN = ~ERR_NONE,

} lcerr_t;


/**
 * @public
 * @brief assert helper
 * @param[in] e error
 * @retval int true on non-error
 */
static inline int
  is_ok( lcerr_t e )
{
  return e == ERR_NONE;
}

typedef struct lc_t lc_t;  /**< fwd decl */

/**
 * @internal or @public
 * @brief
 * @param[in] <name> <desc>
 * @param[out] <name> <desc>
 * @retval <type> <desc>
 */
lcerr_t
  lc_open( lc_t **plc,
           uint8_t *pportstr );

/**
 * @internal or @public
 * @brief keepalive nop in case sets and/or gets aren't being performed
 * @param[in] <name> <desc>
 * @param[out] <name> <desc>
 * @retval <type> <desc>
 */
lcerr_t
  lc_tickle( lc_t *plc );

/**
 * @internal or @public
 * @brief
 * @param[in] <name> <desc>
 * @param[out] <name> <desc>
 * @retval <type> <desc>
 */
lcerr_t
  lc_get_setpoint_temp( lc_t *plc,
                        float *pspt );

/**
 * @internal or @public
 * @brief
 * @param[in] <name> <desc>
 * @param[out] <name> <desc>
 * @retval <type> <desc>
 */
lcerr_t
  lc_set_setpoint_temp( lc_t *plc,
                        float spt );

/**
 * @internal or @public
 * @brief
 * @param[in] <name> <desc>
 * @param[out] <name> <desc>
 * @retval <type> <desc>
 */
lcerr_t
  lc_get_heater_temp( lc_t *plc,
                      float *pht );

/**
 * @internal or @public
 * @brief
 * @param[in] <name> <desc>
 * @param[out] <name> <desc>
 * @retval <type> <desc>
 */
lcerr_t
  lc_get_food_temp( lc_t *plc,
                    float *pft );

/**
 * @internal or @public
 * @brief
 * @param[in] <name> <desc>
 * @param[out] <name> <desc>
 * @retval <type> <desc>
 */
lcerr_t
  lc_close( lc_t **plc );

#ifdef __cplusplus
}
#endif

#endif /* _lc_h_ */
