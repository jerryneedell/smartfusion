/*******************************************************************************
 * (c) Copyright 2012-2015 Microsemi SoC Products Group. All rights reserved.
 *
 *  Microsemi Smartfusion2 USB Host logical driver layer Implementation.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBH-MSC class driver.
 *
 * USBH-MSC driver public API.
 *
 *
 * SVN $Revision: 7461 $
 * SVN $Date: 2015-06-11 13:48:10 +0530 (Thu, 11 Jun 2015) $
 */

/*=========================================================================*//**
  @mainpage Smartfusion2 MSS USB driver
                USBH-MSC driver

  @section intro_sec Introduction
  The Mass Storage Class host driver implements the USB host controller as per
  the USB MSC class specified by the USB-IF. This driver enables easy detection
  and data transfers with the attached USB mass storage devices.

  This driver implements the MSC class using Bulk Only transport (BOT). One
  BULK IN and one BULK OUT endpoints are used to implement BOT. This MSC class
  host supports one LUN.

  This driver uses the USBH driver interface functions to implement the USB MSC
  host. This host controller will be able to work with all the USB flash drives
  which fall in "Memory stick" category.

  This driver passes the USB-IF USB2.0 compliance test for high speed embedded
  host. Please refer the user's guide of the Smartfusion2 MSS USB driver for
  the Targeted peripheral list (TPL) used for the compliance test.

  Though a limited number of targeted peripherals are used for compliance test,
  in general, this driver will be able to access any MSC class device which fall
  in the �memory stick� category.

  @section theory_op Theory of Operation
  The following steps are involved in the operation of USBH-MSC driver:
        �    Configuration
        �    Initialization
        �    Enumeration
        �    Class Specific requests
        �    data transfer

  Configuration
  The MSS USB driver must first be configured in the USB Host mode using the
  MSS_USB_HOST_MODE to use this driver. No other configuration is necessary.

  Initialization
  The MSC class driver must be initialized using the MSS_USBH_MSC_init() function.
  A pointer to the structure of type mss_usbh_msc_user_cb_t must be provided as
  parameter with this function. This pointer is used to call the application
  call-back functions by the USBH-MSC driver. These call-back functions can be
  used by the application to provide error/status messages to the user or for
  performing application specific handling of the events.

  Class Specific requests
  The class specific requests are handled internally by this driver. Both the
  GET_MAX_LUN and the RESET_RECOVERY requests are supported. The GET_MAX_LUN
  request is executed after the enumeration process to find out the number of
  LUNs on the attached device. This driver supports one LUN (index 0).

  Application call-back interface
  After the USBH-Class driver is assigned to the attached device by the USBH
  driver, as indicated by usbh_class_allocate call-back, this driver can take
  over the communication with the attached device. This driver will then extend
  the enumeration process by requesting the class specific information from the
  device. During this process it will also call the call-back functions to
  indicate the progress of the enumeration process. These call-back functions
  must be implemented by application. These call-back functions can be used to
  take application specific action on specific event or can be used to provide
  error/status messages to the user. A type mss_usbh_msc_user_cb_t is provided
  by this driver which has all these call-back functions as its elements.
  Implementing these call-back functions is optional.

  Data transfer
  The MSC class driver performs the data transfers using one BULK IN endpoint
  and one BULK OUT endpoint. The data transfers use transparent SCSI commands
  and follow the BoT specification. The BoT read/write operations happen on
  logical units of fixed block size. During initialization this driver takes
  care of finding all information about the attached device. The
  MSS_USBH_MSC_get_sector_count() and MSS_USBH_MSC_get_sector_size() functions
  can be used to find out the sector count and the sector size on the attached
  MSC class device.

  Most of the times the read (READ_10) and write (WRITE_10) are the only SCSI
  operations that the application needs to do. The MSS_USBH_MSC_read() and
  MSS_USBH_MSC_write() functions are provided for this purpose. The application
  can use the MSS_USBH_MSC_scsi_req() function if it needs to perform any other
  SCSI operation. This function can be used for READ_10 and WRITE_10 commands as
  well. Once the SCSI request is initiated using any of the above functions,
  the MSS_USBH_MSC_is_scsi_req_complete() function can be used to find out when
  the operation is complete.

  The USBH driver supports multi-packet Bulk transfers. The USBH-MSC driver makes
  full use of this feature by passing on the multi-sector transfers from the
  application to the USBH driver. To free up the application from transferring
  data to/from MSS USB FIFO, this driver can configure USBH driver to use the
  MSS USB internal DMA.

  The MSS_USBH_MSC_construct_cbw_cb10byte() and MSS_USBH_MSC_construct_cbw_cb6byte()
  functions are provided so that the user can easily prepare the CBW format
  buffer by providing appropriate parameters instead of manually creating the
  CBW command buffer.

  Data transfer throughput
  To know the efficiency of the data transfers of the SmartFusion2 MSS USB mass
  storage class host a throughput analysis has been performed. The aim of this
  test is to find the maximum possible data transfer rate when MSS USB host is
  communicating with a Mass storage class device.

  USB2.0 High speed operates at 480 MBits/sec. USB being a serial bus, the basic
  packet format used for communication (Token-data-ack format, EOP packets, SOF
  packets etc.) causes protocol overhead which reduces the actual data throughput
  on the bus. USB2.0 spec chapter 5 describes this in details. On top of this,
  the transfer protocol used by the USB MSC class (SCSI protocol) adds to the
  protocol overhead because of the command-data-ack format of the SCSI protocol.
  The USB transfer throughput in case of Mass storage class depends on different
  software/hardware components involved. The involved components which can affect
  the data transfer throughput are: the MSS USB host, the file system used by
  the application, the transfer size selection by the application, efficiency of
  the USB driver running on the attached device and the flash media device
  read/write performance.

  To get the throughput numbers purely for the MSS USB host driver, the test
  setup does not use the file system and the standard pen drives. The test setup
  uses SF2 host connected to the SF2 device. This SF2 device is a dedicated test
  device which enumerates as a MSC class device and uses eSRAM as storage media
  for fastest possible read/write performance.

  The file system implementation itself can have an effect on throughput due to
  its scheduling of the transfer, transfer size selection and the non-optimized
  implementation. In the test setup, after the device is enumerated, we directly
  use the MSS_USBH_MSC_write() and MSS_USBH_MSC_read()  functions so that the
  file system involvement is eliminated. We use a test device which uses the
  USBD driver on SmartFusion2 Devkit which emulates the USBD-MSC device using
  the eSRAM as the storage media, this way we eliminate the Flash memory device
  from the test system. The table below provides the details of the maximum
  throughput that can be achieved with this version of the USBH device driver.

  For comparison of the data throughput, the throughput number for SmartFusion2
  MSS USB mass storage class host communicating with a standard Sandisk 8Gb and
  standard Sandisk 8Gb pen drive is also provided.

    Test setup                       Transfer Type           MBits/Sec

    SF2 Host -
    SF2 Device                       SCSI Read              32.768
    XFR size                         SCSI Write             32.768
    (512 bytes)

    SF2 Host -
    SF2 Device                       SCSI Read              145.635555
    XFR size                         SCSI Write             145.635555
    (4096 bytes)

    SF2 Host -
    SF2 Device                       SCSI Read              242.725925
    XFR size                         SCSI Write             222.155932
    (16384 bytes)

    SF2 Host -
    SF2 Device                       SCSI Read              262.144
    XFR size                         SCSI Write             240.499082
    (32768 bytes)

    SF2 Host -
    Scandisk Device                  SCSI Read              209.7152
    XFR size                         SCSI Write             98.922264
    (32768 bytes)

    SF2 Host -
    Scandisk Device                  SCSI Read              59.578181
    XFR size                         SCSI Write             20.48
    (4096 bytes)

    SF2 Host -
    Kingston Device                  SCSI Read              137.970526
    XFR size                         SCSI Write             77.101176
    (32768 bytes)

    SF2 Host -
    Kingston Device                  SCSI Read              57.487719
    XFR size                         SCSI Write             88.652162
    (4096 bytes)


 *//*=========================================================================*/

#ifndef __MSS_USB_HOST_MSC_H_
#define __MSS_USB_HOST_MSC_H_

#include <stdint.h>
#include "mss_usb_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MSS_USB_HOST_ENABLED

/***************************************************************************//**
 Types exported from USBH-MSC driver
*/

/***************************************************************************//**
  mss_usbh_msc_err_code_t
  The mss_usbh_msc_err_code_t provides a type to identify the error occurred
  during retrieving configuration and other class specific descriptors from the
  attached MSC class device. This type can be used with msch_error  call-back
  function element of mss_usbh_msc_user_cb_t type to identify the exact cause
  of the error. The meaning of the constants is as described below

    USBH_MSC_EP_NOT_VALID        -- Indicates that the endpoint information
                                   retrieved from the attached device was not
                                   consistent with the MSC class.

    USBH_MSC_CLR_CEP_STALL_ERROR-- Indicates that the host controller was not
                                   able to clear the stall condition on the
                                   attached device even after multiple retries.

    USBH_MSC_SECTOR_SIZE_NOT_SUPPORTED -- Indicates that the attached device
                                   sector size is not supported by this driver.
*/
typedef enum {
    USBH_MSC_NO_ERROR = 0,
    USBH_MSC_EP_NOT_VALID = -1,
    USBH_MSC_CLR_CEP_STALL_ERROR = -2,
    USBH_MSC_SECTOR_SIZE_NOT_SUPPORTED = -3,
    USBH_MSC_WRONG_DESCR = -4,
} mss_usbh_msc_err_code_t;

/***************************************************************************//**
  mss_usbh_msc_state_t
  The mss_usbh_msc_state_t provides a type for the states of operation of the
  USBH-MSC driver. Most of the states are internally used by the USBH-MSC driver
  during the enumeration process. The USBH-MSC driver is ready to perform data
  transfers with the attached device when the driver is in state
  USBH_MSC_DEVICE_READY. The USBH_MSC_ERROR state indicates that the error was
  detected either during enumeration or during the normal data transfer
  operations with the attached device even after retries.
*/
typedef enum {
    USBH_MSC_IDLE,
    USBH_MSC_GET_CLASS_DESCR,
    USBH_MSC_WAIT_GET_CLASS_DESCR,
    USBH_MSC_SET_CONFIG,
    USBH_MSC_WAIT_SET_CONFIG,
    USBH_MSC_WAIT_DEV_SETTLE,           //30ms waiting time for device to settle
    USBH_MSC_GET_MAX_LUN,
    USBH_MSC_WAIT_GET_MAX_LUN,
    USBH_MSC_CLR_CEP_STALL,
    USBH_MSC_WAIT_CLR_CEP_STALL,
    USBH_MSC_CONFIG_BULK_ENDPOINTS,

    USBH_MSC_TEST_UNIT_READY_CPHASE,
    USBH_MSC_TEST_UNIT_READY_SPHASE,
    USBH_MSC_TEST_UNIT_READY_WAITCOMPLETE,

    USBH_MSC_SCSI_INQUIRY_CPHASE,
    USBH_MSC_SCSI_INQUIRY_DPHASE,
    USBH_MSC_SCSI_INQUIRY_SPHASE,
    USBH_MSC_SCSI_INQUIRY_WAITCOMPLETE,

    USBH_MSC_SCSI_REQSENSE_CPHASE,
    USBH_MSC_SCSI_REQSENSE_DPHASE,
    USBH_MSC_SCSI_REQSENSE_SPHASE,
    USBH_MSC_SCSI_REQSENSE_WAITCOMPLETE,

    USBH_MSC_SCSI_READ_CAPACITY_CPHASE,
    USBH_MSC_SCSI_READ_CAPACITY_DPHASE,
    USBH_MSC_SCSI_READ_CAPACITY_SPHASE,
    USBH_MSC_SCSI_READ_CAPACITY_WAITCOMPLETE,

    USBH_MSC_DEVICE_READY,
    USBH_MSC_BOT_RETRY,
    USBH_MSC_ERROR
} mss_usbh_msc_state_t;

/***************************************************************************//**
 Data structures exported by USBH-MSC driver
 */

/***************************************************************************//**
  mss_usbh_msc_user_cb_t
  The mss_usbh_msc_user_cb_t provides the prototype for all the call-back
  functions which must be implemented by the user application. The user
  application must define and initialize a structure of this type and provide
  the address of that structure as parameter to the MSS_USBH_MSC_init() function.

  msch_valid_config
  The function pointed by the msch_valid_config function pointer will be called
  to indicate that a valid MSC class configuration was found on the attached
  device and the device is configured for this configuration.

  msch_tdev_ready
  The function pointed by the msch_tdev_ready function pointer is called when
  this driver is able to retrieve all the MSC class specific information
  (including sector size and sector number) from the attached device and is
  ready to perform the data transfers.

  msch_driver_released
  The function pointed by the msch_driver_released function pointer is called to
  indicate to the application that this driver is released by the USBH driver.
  This could be either because the MSC class device is now detached or there is
  permanent error with the USBH driver.

  msch_error
  The function pointed by the msch_error function pointer is called to indicate
  that there was an error while retrieving the class specific descriptor
  information from the attached MSC class device. The error_code parameter
  indicates the exact cause of the error.
*/
typedef struct mss_usbh_msc_user_cb
{
  void (*msch_valid_config)(void);
  void (*msch_tdev_ready)(void);
  void (*msch_driver_released)(void);
  void (*msch_error)(int8_t error_code);
} mss_usbh_msc_user_cb_t;

/***************************************************************************//**
  msd_cbw_t
  The msd_cbw_t type provides the prototype for the Command Block Wrapper (CBW)
  as defined in Universal Serial Bus Mass Storage Class Bulk-Only Transport
  Revision 1.0. This type can be used by the application to create buffer of
  this type for creating the CBW which can then be passed on to functions
  provided by this driver as parameter.

  dCBWSignature
  Signature that helps identify this data packet as a CBW. The signature field
  shall contain the value 43425355h (little endian), indicating a CBW.

  dCBWTag
  A Command Block Tag sent by the host. The device shall echo the contents of
  this field back to the host in the dCSWTagfield of the associated CSW. The
  dCSWTagpositively associates a CSW with the corresponding CBW.

  dCBWDataTransferLength
  The number of bytes of data that the host expects to transfer on the Bulk-In
  or Bulk-Out endpoint (as indicated by the Direction-bit) during the execution
  of this command. If this field is zero, the device and the host shall transfer
  no data between the CBW and the associated CSW, and the device shall ignore
  the value of the Direction-bit in bmCBWFlags.

  bCBWFlags
  The bits of this field are defined as follows:
    Bit 7       Direction- the device shall ignore this bit if the
                dCBWDataTransferLengthfield is zero, otherwise:
                0 = Data-Out from host to the device,
                1 = Data-In from the device to the host.

    Bit 6       Obsolete. The host shall set this bit to zero.
    Bits 5..0     Reserved - the host shall set these bits to zero.

  bCBWLUN
  The device Logical Unit Number (LUN) to which the command block is being sent.
  For devices that support multiple LUNs, the host shall place into this field
  the LUN to which this command block is addressed. Otherwise, the host shall
  set this field to zero.

  bCBWCBLength
  The valid length of the CBWCBin bytes. This defines the valid length of the
  command block. The only legal values are 1 through 16 (01h through 10h).
  All other values are reserved.

  CBWCB[16]
  The command block to be executed by the device. The device shall interpret the
  first bCBWCBLength bytes in this field as a command block as defined by the
  command set identified by bInterfaceSubClass. If the command set supported by
  the device uses command blocks of fewer than 16 (10h) bytes in length, the
  significant bytes shall be transferred first, beginning with the byte at
  offset 15 (0Fh). The device shall ignore the content of the CBWCBfield past
  the byte at offset (15 + bCBWCBLength- 1)
*/
typedef struct {
 uint32_t dCBWSignature;
 uint32_t dCBWTag;
 uint32_t dCBWDataTransferLength;
 uint8_t bCBWFlags;
 uint8_t bCBWLUN;
 uint8_t bCBWCBLength;
 uint8_t CBWCB[16];
} msd_cbw_t;

/***************************************************************************//**
  msd_csw_t
    The msd_csw_t type provides the prototype for the Command Status Wrapper (CSW)
    as defined in Universal Serial Bus Mass Storage Class Bulk-Only Transport
    Revision 1.0. This type can be used by application to create buffer of this
    type for creating CSW which can then be passed on to APIs provided by this
    driver as parameter.

  dCSWSignature
    Signature that helps identify this data packet as a CSW. The signature field
    shall contain the value 53425355h (little endian), indicating CSW.

  dCSWTag
    The device shall set this field to the value received in the dCBWTag of the
    associated CBW.

  dCSWDataResidue
    For Data-Out the device shall report in the dCSWDataResiduethe difference
    between the amount of data expected as stated in the dCBWDataTransferLength,
    and the actual amount of data processed by the device. For Data-In the device
    shall report in the dCSWDataResiduethe difference between the amount of data
    expected as stated in the dCBWDataTransferLengthand the actual amount of
    relevant data sent by the device. The dCSWDataResidueshall not exceed the
    value sent in the dCBWDataTransferLength.

  dCSWStatus
    bCSWStatusindicates the success or failure of the command. The device shall
    set this byte to zero if the command completed successfully. A non-zero value
    shall indicate a failure during command execution according to the following
    table:
        Value               Description
        00h                 Command Passed ("good status")
        01h                 Command Failed
        02h                 Phase Error
        03h and 04h         Reserved (Obsolete)
        05h to FFh          Reserved
*/
typedef struct {
 uint32_t dCSWSignature;
 uint32_t dCSWTag;
 uint32_t dCSWDataResidue;
 uint32_t dCSWStatus;
} msd_csw_t;

/*******************************************************************************
 * EXPORTED APIs from USBH-MSC driver
 *******************************************************************************/

/***************************************************************************//**
  @brief MSS_USBH_MSC_init()
  The MSS_USBH_MSC_init() function must be used by the application to initialize
  the USBH-MSC driver. This function must be called before any other function of
  the USBH-MSC driver.

  @param user_cb
  The user_cb parameter provides a pointer to the structure of type
  mss_usbh_msc_user_cb_t. This pointer is used to call the application call-back
  functions by the USBH-MSC driver. These call-back functions can be used by the
  application to provide error/status messages to the user or for performing the
  application specific handling of the events.

  @return
    This function does not return a value.

  Example:
  @code
    //Initialize the USBH driver
    MSS_USBH_init(&MSS_USBH_user_cb);

    //Initialize the USBH-MSC driver
    MSS_USBH_MSC_init(&MSS_USBH_MSC_user_cb);


//     Get Class driver handle from the USBH-MSC class driver and register it with
//     the USBH driver.
//     On Device attachment, USBH driver will look for Device information through
//     Descriptors and if match it with the Class driver using this Handle.

    MSS_USBH_register_class_driver(MSS_USBH_MSC_get_handle());

    @endcode
*/
void
MSS_USBH_MSC_init
(
    mss_usbh_msc_user_cb_t* user_sb
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_task()
  The MSS_USBH_MSC_task() function is the main task of the USBH-MSC driver.
  This function monitors the events from the USBH driver as well as the user
  application and makes decisions. This function must be called repeatedly by
  the application to allow the USBH-MSC driver to perform the housekeeping tasks.
  A timer/scheduler can be used to call this function at regular intervals or
  it can be called from the main continuous foreground loop of the application.

  @param
    This function does not take any parameters.

  @return
    This function does not return a value.

  Example:
  @code
    #define SYS_TICK_LOAD_VALUE                             48000u    //For 1ms
    int main()
    {
        //Initialize SysTick
        SysTick_Config(SYS_TICK_LOAD_VALUE);
        NVIC_EnableIRQ(SysTick_IRQn);
    }
    void SysTick_Handler(void)
    {
        MSS_USBH_MSC_task();
    }
  @endcode
*/
void
MSS_USBH_MSC_task
(
    void
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_get_handle()
  The MSS_USBH_MSC_get_handle() function must be used by the application to get
  the handle from the USBH-MSC driver. This handle must then be used to register
  this driver with the USBH driver.

  @param
    This function does not take any parameters.

  @return
    This function returns a pointer to the class-handle structure.

  Example:
  @code
    //Initialize the USBH driver
    MSS_USBH_init(&MSS_USBH_user_cb);

    //Initialize the USBH-MSC driver
    MSS_USBH_MSC_init(&MSS_USBH_MSC_user_cb);

//    Get Class driver handle from the USBH-MSC class driver and register it with
//    the USBH driver.
//    On Device attachment, USBH driver will look for Device information through
//    descriptors and match it with the Class driver using this Handle.

    MSS_USBH_register_class_driver(MSS_USBH_MSC_get_handle());

    @endcode
*/
void*
MSS_USBH_MSC_get_handle
(
    void
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_get_state()
  The MSS_USBH_MSC_get_state() function can be used to find out the current state
  of the USBH-MSC driver. This information can be used by the application to check
  the readiness of the USBH-MSC driver to start the data transfers. The USBH-MSC
  driver can perform data transfers only when it is in the USBH_MSC_DEVICE_READY
  state.

  @param
  This function does not take any parameters.

  @return
  This function returns a value of type mss_usbh_msc_state_t indicating the
  current state of the USBH-MSC driver.

  Example:
  @code
    if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
            {
                *result = MSS_USBH_MSC_get_sector_count();
                return RES_OK;
            }
            else if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
            {
                *result = 0u;
                return RES_NOTRDY;
    }
  @endcode
*/
mss_usbh_msc_state_t
MSS_USBH_MSC_get_state
(
    void
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_read()
  The MSS_USBH_MSC_read() function can be used to read data from the attached
  mass storage device. This is a non-blocking function. The function prepares
  the MSS USB for the USB IN transfer. Once prepared, the MSS USB will start
  this IN transfer depending on the configuration of the IN pipe on which this
  transfer is occurring. The USBH-MSC driver takes care of the allocation and
  configuration of the IN pipe during enumeration process depending on the
  attached mass storage device. After preparing the IN pipe for the IN transfer,
  the MSS_USBH_MSC_is_scsi_req_complete() function can be used to find out the
  completion of the transfer.

  @param buf
  The buf parameter is a pointer to the buffer where the data received in the IN
  transfer from the attached MSC class device is stored.

  @param sector
  The sector parameter indicates the sector number (logical block address) on
  the mass storage device starting from which the data is to be read.

  @param count
  The count parameter indicates the number of sectors to be read.

  @return
  This function returns a zero value when execution was successful.

  Example:
  @code
        DRESULT disk_read ( BYTE drv,        // Physical drive number (0..)
            BYTE *buff,        // Data buffer to store read data
            DWORD sector,    // Sector address (LBA)
            BYTE count        // Number of sectors to read (1..255) )
        {
            if(0u != drv)
                return(RES_ERROR);
            if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
            {
                return(RES_NOTRDY);
            }
            else if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
            {
                MSS_USBH_MSC_read(buff, sector, count);
                while(MSS_USBH_MSC_is_scsi_req_complete());
                return(RES_OK);
            }
            else
                return(RES_ERROR);
        }
  @endcode
*/
int8_t
MSS_USBH_MSC_read
(
    uint8_t* buf,
    uint32_t sector,
    uint32_t count
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_write()
  The MSS_USBH_MSC_write() function can be used to write data to the attached
  mass storage device. This is a non-blocking function. The function readies the
  MSS USB for the USB OUT transfer. Once ready, the MSS USB will start this OUT
  transfer depending on the configuration of the OUT pipe on which this transfer
  is occurring. The USBH-MSC driver takes care of the allocation and
  configuration of the OUT pipe during enumeration process depending on the
  attached mass storage device. After starting the OUT transfer on the specified
  OUT pipe, the MSS_USBH_MSC_is_scsi_req_complete() function can be used to find
  out the completion of the transfer.

  @param buf
  The buf parameter is a pointer to the buffer from which data need to be
  transmitted to the attached MSC class device.

  @param sector
  The sector parameter indicates the sector number (logical block address) on
  the mass storage device starting from which the data is to be written.

  @param count
  The count parameter indicates the number of sectors to be written.

  @return
  This function returns a zero value when execution was successful.

  Example:
  @code
    DRESULT disk_write ( BYTE pdrv,        // Physical drive number (0..)
        const BYTE *buff,    // Data to be written
        DWORD sector,        // Sector address (LBA)
        BYTE count            // Number of sectors to write (1..255) )
    {
        if(0u != pdrv)
            return(RES_ERROR);
        if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
        {
            return(RES_NOTRDY);
        }
        else if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
        {
            MSS_USBH_MSC_write((uint8_t*)buff, sector, count);
            while(MSS_USBH_MSC_is_scsi_req_complete());
            return(RES_OK);
        }
        else
            return(RES_ERROR);
    }

  @endcode
*/
int8_t
MSS_USBH_MSC_write
(
    uint8_t* buf,
    uint32_t sector,
    uint32_t count
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_get_sector_count()
  The MSS_USBH_MSC_get_sector_count() function can be used to find out the
  number of sectors (logical blocks) available on the attached MSC class device.

  @param
  This function does not take any parameters.

  @return
  This function returns a value of type uint32_t indicating the number of
  sectors (logical blocks) available on the attached MSC class device.

  Example:
  @code
        DRESULT disk_ioctl (
            BYTE pdrv,        // Physical drive number (0..)
            BYTE ctrl,        // Control code
            void *buff        // Buffer to send/receive control data
        )
        {
            UINT *result = (UINT *)buff;

            switch (ctrl) {
            case GET_SECTOR_COUNT:
                if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
                {
                    *result = MSS_USBH_MSC_get_sector_count();
                    return RES_OK;
                }
                else if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
                {
                    *result = 0u;
                    return RES_NOTRDY;
                }
                else
                {
                    *result = 0u;
                    return RES_ERROR;
                }
                break;

            default:
                return RES_NOTRDY;
            }
        }
  @endcode
*/
uint32_t
MSS_USBH_MSC_get_sector_count
(
    void
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_get_sector_size()
  The MSS_USBH_MSC_get_sector_size() function can be used to find out the size
  of a sector (in bytes) on the attached MSC class device.

  @param
  This function does not take any parameters.

  @return
  This function returns a value of type uint32_t indicating the sector size
  (in bytes) on the attached MSC class device.

  Example:
  @code
        DRESULT disk_ioctl (
            BYTE pdrv,        // Physical drive number (0..)
            BYTE ctrl,        // Control code
            void *buff        // Buffer to send/receive control data
        )
        {
            UINT *result = (UINT *)buff;

            switch (ctrl) {
        case GET_SECTOR_SIZE:
                if(USBH_MSC_DEVICE_READY == MSS_USBH_MSC_get_state())
                {
                    *result = MSS_USBH_MSC_get_sector_size();
                    return RES_OK;
                }
                else if(USBH_MSC_DEVICE_READY < MSS_USBH_MSC_get_state())
                {
                    *result = 0u;
                    return RES_NOTRDY;
                }
                else
                {
                    *result = 0u;
                    return RES_ERROR;
                }
                break;

            default:
                return RES_NOTRDY;
            }
        }
  @endcode
*/
uint32_t
MSS_USBH_MSC_get_sector_size
(
    void
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_construct_cbw_cb10byte()
  The MSS_USBH_MSC_construct_cbw_cb10byte() function can be used to create the
  SCSI request command block wrapper (CBW) as per MSC class which has command
  block(CB) of length 10bytes.

  @param command_opcode
  The command_opcode parameter provides the transparent SCSI command code.

  @param lun
  The lun parameter indicates the logical unit number on the attached MSC class
  device.

  @param lb_addr
  The lb_addr parameter provides the logical block address on which the operation
  indicated by the command_opcode parameter is applicable. For the commands where
  logical block address is not applicable, a zero value must be provided.

  @param num_of_lb
  The num_of_addr parameter provides the number of logical blocks on which the
  operation indicated by the command_opcode parameter is applicable. For the
  commands where logical block address is not applicable, a zero value must be
  provided.

  @param lb_size
  The lb_size parameter provides the size of the logical block on the attached
  MSC class device.

  @param buf
  The buf parameter provides the pointer to the buffer where the formatted SCSI
  command is to be stored.

  @return
    This function does not return any value.

  Example:
  @code
        int8_t
        MSS_USBH_MSC_read(uint8_t* buf, uint32_t sector, uint32_t count)
        {
            MSS_USBH_MSC_construct_cbw_cb10byte(USB_MSC_SCSI_READ_10,
                                                0u,
                                                sector,
                                                count,
                                                512u,
                                                &g_bot_cbw);


        MSS_USBH_MSC_scsi_req((uint8_t*)&g_bot_cbw,
                                  buf,
                                  (count*512u),
                                  (uint8_t*)&g_bot_csw);
            return(0);
        }
  @endcode
*/
void
MSS_USBH_MSC_construct_cbw_cb10byte
(
    uint8_t command_opcode,
    uint8_t lun,
    uint32_t lb_addr,
    uint16_t num_of_lb,
    uint16_t lb_size,
    msd_cbw_t* buf
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_construct_cbw_cb6byte()
  The MSS_USBH_MSC_construct_cbw_cb6byte() function can be used to create the
  SCSI request command block wrapper (CBW) as per MSC class which has command
  block(CB) of length 6 bytes.

  @param command_opcode
  The command_opcode parameter provides the transparent SCSI command code.

  @param xfr_length
  The xfr_length parameter provides the number of bytes to be transferred in the
  data phase of the command.

  @param buf
  The buf parameter provides the pointer to the buffer where the formatted SCSI
  command is to be stored.

  @return
  This function does not return any value.

  Example:
  @code
            MSS_USBH_MSC_construct_cbw_cb6byte(USB_MSC_SCSI_TEST_UNIT_READY,
                                               0u,
                                               &g_bot_cbw);

            MSS_USBH_write_out_pipe(g_msd_tdev_addr,
                                    USBH_MSC_BULK_TX_PIPE,
                                    g_tdev_out_ep.num,
                                    g_tdev_out_ep.maxpktsz,
                                    (uint8_t*)&g_bot_cbw,
                                    31u); //std_cbw_len

@endcode
*/
void
MSS_USBH_MSC_construct_cbw_cb6byte
(
    uint8_t command_opcode,
    uint32_t data_xfr_len,
    msd_cbw_t* buf
);

/*Add DataDir parameter*/
/***************************************************************************//**
  @brief MSS_USBH_MSC_scsi_req()
  The MSS_USBH_MSC_scsi_req() function can be used to execute any SCSI command
  required by the MSC class  on the attached MSC device. In most cases using
  the MSS_USBH_MSC_write() and MSS_USBH_MSC_read() functions is enough for the
  application. However, if the application wants to execute other SCSI commands
  it can use the MSS_USBH_MSC_scsi_req() function. This function can be used as
  alternative to MSS_USBH_MSC_write() and MSS_USBH_MSC_read() functions.

  The MSS_USBH_MSC_is_scsi_req_complete() function can be used to find out when
  the transfer started using this function is complete.

  @param command_buf
  The command_buf parameter provides the pointer to the buffer where the SCSI
  command (CBW format) to be executed is stored.

  @param data_buf
  The data_buf parameter provides the pointer to the data buffer which is to be
  used in the data phase of the command. This parameter is the source of the
  data when the data direction is from the host to the device. This parameter is
  the destination for the data when the data direction is from the device to
  the host. This function extracts the data direction from the CBW format
  command provided using command_buf parameter.

  @param data_buf_len
  The data_buf_len parameter indicates the number of bytes to be transferred in
  the data phase of the current command.

  @param status_buf
  The status_buf parameter provides the pointer to the buffer where the status
  (CSW format) received from the attached MSC device for the current SCSI
  operation is to be stored.

  @return
    This function returns zero value when successfully executed.

  Example:
  @code
        int8_t
        MSS_USBH_MSC_read
        (
            uint8_t* buf,
            uint32_t sector,
            uint32_t count
        )
        {
            MSS_USBH_MSC_construct_cbw_cb10byte(USB_MSC_SCSI_READ_10,
                                                0u,
                                                sector,
                                                count,
                                                512u,
                                                &g_bot_cbw);

            MSS_USBH_MSC_scsi_req((uint8_t*)&g_bot_cbw,
                                  buf,
                                  (count*512u),
                                  (uint8_t*)&g_bot_csw);
            return(0);
        }
  @endcode
*/
uint8_t
MSS_USBH_MSC_scsi_req
(
    uint8_t* command_buf,   //always31bytes
    uint8_t* data_buf,
    uint32_t data_buf_len,
    uint8_t* status_buf     //status always 13bytes
);

/***************************************************************************//**
  @brief MSS_USBH_MSC_is_scsi_req_complete()
  The MSS_USBH_MSC_is_scsi_req_complete() function must be used to find out
  whether the SCSI request initiated using MSS_USBH_MSC_scsi_req() or
  MSS_USBH_MSC_read() or MSS_USBH_MSC_write() function is complete.

  @param
  This function does not take any parameters.

  @return
  This function returns zero value when the current command is completed.

  Example:
  @code
        MSS_USBH_MSC_write((uint8_t*)buff, sector, count);
        while(MSS_USBH_MSC_is_scsi_req_complete());
        return(RES_OK);

  @endcode
*/
uint8_t
MSS_USBH_MSC_is_scsi_req_complete
(
    void
);

#endif  //MSS_USB_HOST_ENABLED

#ifdef __cplusplus
}
#endif

#endif  /* __MSS_USB_HOST_MSC_H_ */

