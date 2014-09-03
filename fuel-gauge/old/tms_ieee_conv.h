/* $RCSfile: tms_ieee_conv.h,v $ $Revision: 1.1 $
 * $Date: 2004/03/01 20:15:00 $ $Author: joel $
 *
 * Real Time DSP Hardware for MEMS Array
 * Joel J. Fuster <j@fuster.org>
 * University of Florida
 *
 * Functions for conversion between TMS and IEEE floating point formats.
 * Code taken with thanks from implementation written by Scott Morrison.
 *
 */

#if !defined(uint32)
       typedef unsigned int        uint32;
#endif

uint32 ieee_to_tms(const float floatIn);
float tms_to_ieee(const uint32 c30in);

struct c30float
{
   signed   int mantissa:23;
 unsigned int sign:1;
  signed   int exponent:8;
};

struct ieeefloat
{
        unsigned int mantissa:23;
    unsigned int exponent:8;
 unsigned int sign:1;
};

union ieee_union            /* IEEE CAN BE TREATED AS AN INT OR A STRUCT */
{
 unsigned long               hex;
    float                                flt;
     struct        ieeefloat        str;
} ieee;

union c30_union            /* C30 CAN BE TREATED AS A FLOAT OR A STRUCT */
{
  unsigned long                hex;
     struct        c30float str;
} c30;
