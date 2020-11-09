/*=============================================================*/
/* Created by Microsemi SmartDesign Fri Nov 06 13:22:59 2020   */
/*                                                             */
/* Warning: Do not modify this file, it may lead to unexpected */
/*          functional failures in your design.                */
/*                                                             */
/*=============================================================*/

/*-------------------------------------------------------------*/
/* SERDESIF_0 Initialization                                   */
/*-------------------------------------------------------------*/

#include <stdint.h>
#include "../../CMSIS/sys_init_cfg_types.h"

const cfg_addr_value_pair_t g_m2s_serdes_0_config[] =
{
    { (uint32_t*)( 0x40028000 + 0x2028 ), 0x10F } /* SYSTEM_CONFIG_PHY_MODE_1 */ ,
    { (uint32_t*)( 0x40028000 + 0x1198 ), 0x30 } /* LANE0_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1000 ), 0x80 } /* LANE0_CR0 */ ,
    { (uint32_t*)( 0x40028000 + 0x1004 ), 0x20 } /* LANE0_ERRCNT_DEC */ ,
    { (uint32_t*)( 0x40028000 + 0x1008 ), 0xF8 } /* LANE0_RXIDLE_MAX_ERRCNT_THR */ ,
    { (uint32_t*)( 0x40028000 + 0x100c ), 0x80 } /* LANE0_IMPED_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1014 ), 0x29 } /* LANE0_PLL_M_N */ ,
    { (uint32_t*)( 0x40028000 + 0x1018 ), 0x20 } /* LANE0_CNT250NS_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1024 ), 0x80 } /* LANE0_TX_AMP_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1030 ), 0x10 } /* LANE0_ENDCALIB_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1034 ), 0x38 } /* LANE0_CALIB_STABILITY_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x103c ), 0x70 } /* LANE0_RX_OFFSET_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x11d4 ), 0x2 } /* LANE0_GEN1_TX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x11d8 ), 0x22 } /* LANE0_GEN1_RX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x1198 ), 0x0 } /* LANE0_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1200 ), 0x1 } /* LANE0_UPDATE_SETTINGS */ ,
    { (uint32_t*)( 0x40028000 + 0x2028 ), 0x20F } /* SYSTEM_CONFIG_PHY_MODE_1 */ ,
    { (uint32_t*)( 0x40028000 + 0x1598 ), 0x30 } /* LANE1_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1400 ), 0x80 } /* LANE1_CR0 */ ,
    { (uint32_t*)( 0x40028000 + 0x1404 ), 0x20 } /* LANE1_ERRCNT_DEC */ ,
    { (uint32_t*)( 0x40028000 + 0x1408 ), 0xF8 } /* LANE1_RXIDLE_MAX_ERRCNT_THR */ ,
    { (uint32_t*)( 0x40028000 + 0x140c ), 0x80 } /* LANE1_IMPED_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1414 ), 0x29 } /* LANE1_PLL_M_N */ ,
    { (uint32_t*)( 0x40028000 + 0x1418 ), 0x20 } /* LANE1_CNT250NS_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1424 ), 0x80 } /* LANE1_TX_AMP_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1430 ), 0x10 } /* LANE1_ENDCALIB_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1434 ), 0x38 } /* LANE1_CALIB_STABILITY_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x143c ), 0x70 } /* LANE1_RX_OFFSET_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x15d4 ), 0x2 } /* LANE1_GEN1_TX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x15d8 ), 0x22 } /* LANE1_GEN1_RX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x1598 ), 0x0 } /* LANE1_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1600 ), 0x1 } /* LANE1_UPDATE_SETTINGS */ ,
    { (uint32_t*)( 0x40028000 + 0x2028 ), 0x40F } /* SYSTEM_CONFIG_PHY_MODE_1 */ ,
    { (uint32_t*)( 0x40028000 + 0x1998 ), 0x30 } /* LANE2_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1800 ), 0x80 } /* LANE2_CR0 */ ,
    { (uint32_t*)( 0x40028000 + 0x1804 ), 0x20 } /* LANE2_ERRCNT_DEC */ ,
    { (uint32_t*)( 0x40028000 + 0x1808 ), 0xF8 } /* LANE2_RXIDLE_MAX_ERRCNT_THR */ ,
    { (uint32_t*)( 0x40028000 + 0x180c ), 0x80 } /* LANE2_IMPED_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1814 ), 0x29 } /* LANE2_PLL_M_N */ ,
    { (uint32_t*)( 0x40028000 + 0x1818 ), 0x20 } /* LANE2_CNT250NS_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1824 ), 0x80 } /* LANE2_TX_AMP_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1830 ), 0x10 } /* LANE2_ENDCALIB_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1834 ), 0x38 } /* LANE2_CALIB_STABILITY_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x183c ), 0x70 } /* LANE2_RX_OFFSET_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x19d4 ), 0x2 } /* LANE2_GEN1_TX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x19d8 ), 0x22 } /* LANE2_GEN1_RX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x1998 ), 0x0 } /* LANE2_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1a00 ), 0x1 } /* LANE2_UPDATE_SETTINGS */ ,
    { (uint32_t*)( 0x40028000 + 0x2028 ), 0x80F } /* SYSTEM_CONFIG_PHY_MODE_1 */ ,
    { (uint32_t*)( 0x40028000 + 0x1d98 ), 0x30 } /* LANE3_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1c00 ), 0x80 } /* LANE3_CR0 */ ,
    { (uint32_t*)( 0x40028000 + 0x1c04 ), 0x20 } /* LANE3_ERRCNT_DEC */ ,
    { (uint32_t*)( 0x40028000 + 0x1c08 ), 0xF8 } /* LANE3_RXIDLE_MAX_ERRCNT_THR */ ,
    { (uint32_t*)( 0x40028000 + 0x1c0c ), 0x80 } /* LANE3_IMPED_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1c14 ), 0x29 } /* LANE3_PLL_M_N */ ,
    { (uint32_t*)( 0x40028000 + 0x1c18 ), 0x20 } /* LANE3_CNT250NS_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1c24 ), 0x80 } /* LANE3_TX_AMP_RATIO */ ,
    { (uint32_t*)( 0x40028000 + 0x1c30 ), 0x10 } /* LANE3_ENDCALIB_MAX */ ,
    { (uint32_t*)( 0x40028000 + 0x1c34 ), 0x38 } /* LANE3_CALIB_STABILITY_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x1c3c ), 0x70 } /* LANE3_RX_OFFSET_COUNT */ ,
    { (uint32_t*)( 0x40028000 + 0x1dd4 ), 0x2 } /* LANE3_GEN1_TX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x1dd8 ), 0x22 } /* LANE3_GEN1_RX_PLL_CCP */ ,
    { (uint32_t*)( 0x40028000 + 0x1d98 ), 0x0 } /* LANE3_PHY_RESET_OVERRIDE */ ,
    { (uint32_t*)( 0x40028000 + 0x1e00 ), 0x1 } /* LANE3_UPDATE_SETTINGS */ ,
    { (uint32_t*)( 0x40028000 + 0x2028 ), 0xF0F } /* SYSTEM_CONFIG_PHY_MODE_1 */ 
};

