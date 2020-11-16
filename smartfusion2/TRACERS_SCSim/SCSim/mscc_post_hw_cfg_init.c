/*******************************************************************************
 * (c) Copyright 2012 Microsemi Corporation.  All rights reserved.
 *
 *  
 *
 * SVN $Revision: 8059 $
 * SVN $Date: 2015-10-20 12:00:25 +0530 (Tue, 20 Oct 2015) $
 */

#include "m2sxxx.h"
 
/*==============================================================================
 * 
 */
void configure_zl30362(void);

/*==============================================================================
 * 
 */
void mscc_post_hw_cfg_init(void)
{
//    SERDESIF_TypeDef * SERDES0 = (SERDESIF_TypeDef *)0x40028000;
//    SERDESIF_TypeDef * SERDES1 = (SERDESIF_TypeDef *)0x4002C000;
    
    /*--------------------------------------------------------------------------
     * Configure external clock synthesizer.
     */
//    configure_zl30362();
#if 0
#if 1 
    /*--------------------------------------------------------------------------
     * Configure SERDES 0 lane 3 for SGMII.
     * Required for using first copper interface of Marvell PHY on SF2 DEV KIT.
     */
    SERDES0->lane[3].GEN1_RX_PLL_CCP = 0x00000022;
    SERDES0->lane[3].GEN1_TX_PLL_CCP = 0x00000002;

    SERDES0->lane[3].CNT250NS_MAX = 0x00000020;
    SERDES0->lane[3].PLL_M_N =  0x00000029;
    SERDES0->lane[3].ENDCALIB_MAX = 0x00000010;
    SERDES0->sys_regs.CONFIG_PHY_MODE_0 = 0x00004444;
    SERDES0->sys_regs.PCLK_SEL = 0x00000030;
    SERDES0->lane[3].TX_PST_RATIO = 0x00000000;
    SERDES0->lane[3].CNT250NS_MAX = 0x00000020;
    SERDES0->lane[3].PLL_M_N = 0x00000029;
    SERDES0->lane[3].ENDCALIB_MAX = 0x00000000;
    SERDES0->sys_regs.CONFIG_PHY_MODE_0 = 0x00004444;
    SERDES0->lane[3].UPDATE_SETTINGS = 0x00000001;
    SERDES0->sys_regs.EPCS_RSTN_SEL = 0x00000001;
    SERDES0->lane[3].TX_PRE_RATIO = 0x00000000;
    SERDES0->lane[3].UPDATE_SETTINGS = 0x00000001;
#else
    /*--------------------------------------------------------------------------
     * Configure SERDES 1 lane 3 for SGMII.
     * Required for using second copper interface of Marvell PHY on SF2 DEV KIT.
     */
    SERDES1->lane[3].GEN1_RX_PLL_CCP = 0x00000022;
    SERDES1->lane[3].GEN1_TX_PLL_CCP = 0x00000002;

    SERDES1->lane[3].CNT250NS_MAX = 0x00000020;
    SERDES1->lane[3].PLL_M_N =  0x00000029;
    SERDES1->lane[3].ENDCALIB_MAX = 0x00000010;
    SERDES1->sys_regs.CONFIG_PHY_MODE_0 = 0x00004444;
    SERDES1->sys_regs.PCLK_SEL = 0x00000030;
    SERDES1->lane[3].TX_PST_RATIO = 0x00000000;
    SERDES1->lane[3].CNT250NS_MAX = 0x00000020;
    SERDES1->lane[3].PLL_M_N = 0x00000029;
    SERDES1->lane[3].ENDCALIB_MAX = 0x00000000;
    SERDES1->sys_regs.CONFIG_PHY_MODE_0 = 0x00004444;
    SERDES1->lane[3].UPDATE_SETTINGS = 0x00000001;
    SERDES1->sys_regs.EPCS_RSTN_SEL = 0x00000001;
    SERDES1->lane[3].TX_PRE_RATIO = 0x00000000;
    SERDES1->lane[3].UPDATE_SETTINGS = 0x00000001;
#endif
#endif
}
