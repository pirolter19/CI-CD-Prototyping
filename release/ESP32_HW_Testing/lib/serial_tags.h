/*
 * File:         serial_tags.h
 * Author:       Philipp Steinwender
 * Created:      26.01.2023
 */

#ifndef SERIAL_TAGS_H
#define SERIAL_TAGS_H

// tags for filtering printed values on uno
const char MOTOR_SERIAL_TAG = 'm';
const char SERVO_SERIAL_TAG = 's';
const char USONIC_DISTANCE_SERIAL_TAG = 'd';

// special chars for serial reader
const char END_CHAR = '\0';
const char NEW_LINE_CHAR = '\n';

#endif // SERIAL_TAGS_H
