/*
 *  weighing_scale_uuids.h : <Write brief>
 */


#ifndef WEIGHING_SCALE_UUIDS_H
#define WEIGHING_SCALE_UUIDS_H

/** Base UUID : 00000000-0000-1000-8000-00805f9b34fb */
#define WEIGHING_SCALE_BASE_UUID        \
{                                       \
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00\
}

/** Weight Service : 000000ff-0000-1000-8000-00805f9b34fb */
#define WEIGHT_READ_SERVICE         0x00ff

/** Weight read data : 0000ff00-0000-1000-8000-00805f9b34fb */
#define WEIGHT_READ_DATA            0xff00


/** Setting Service : 000000ee-0000-1000-8000-00805f9b34fb */
#define SYSTEM_SETTING_SERVICE      0x00ee

/** Forced OTA : 0000ee06-0000-1000-8000-00805f9b34fb */
#define FORCE_OTA                   0xee06    

/** Software version : 0000ee09-0000-1000-8000-00805f9b34fb*/
#define SOFTWARE_VERSION            0xee09

/** Tare to zero : 0000ee0A-0000-1000-8000-00805f9b34fb */
#define TARE_TO_ZERO                0xee0a

/** Battery PERCENT : 0000ee0B-0000-1000-8000-00805f9b34fb */
#define BATTERY_PERCENT             0xee0b

/** Controller Identifier : 0000ee0C-0000-1000-8000-00805f9b34fb */
#define CONTROLLER_IDEDNTIFIER      0xee0c


#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif /* WEIGHING_SCALE_UUIDS_H */

