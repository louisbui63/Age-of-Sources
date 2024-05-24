#include "../selection.h"
#include "unit_function.h"

//! This file is a template for the unit files, every data about the units must
//! be put inside multiline commentaries and nothing else will use this kind of
//! commentary.

//! The first text is the name of the unit. It must not be more than 255
//! characters long.
/*Tanuki*/

//! The second text is the hp of the unit, it must a be positive decimal number
//! smaller or equal than 65535
/*200*/

//! The third text is the b_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*20*/

//! The fourth text is the p_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The fifth text is the s_dam of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*0*/

//! The sixth text is the b_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*20*/

//! The seventh text is the p_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*10*/

//! The eigth text is the s_def of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*5*/

//! The ninth text is the rg of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*5*/

//! The tenth text is the sp of the unit, it must a be positive decimal
//! number smaller or equal than 65535
/*10*/

//! The elventh text is the w of the unit's sprite, it must a be positive
//! decimal number smaller or equal than 65535
/*32*/

//! The twelfth text is the h of the unit's sprite, it must a be positive
//! decimal number smaller or equal than 65535
/*32*/

//! The thirteenth text is the unit's sprite path, it must be less than 255
//! characters long.
/*asset/sprites/tanuki.bmp*/

//! The fourtennth text is the unit's description, it must be less than 1024
//! characters long.
/*A cute furball that can also build stuff*/

ClickEvent tanuki_grid(__attribute__((unused)) World *w,
                       __attribute__((unused)) int slot,
                       __attribute__((unused)) Entity *e) {
  return empty_click_event;
}
