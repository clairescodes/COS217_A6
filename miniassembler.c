/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Bob Dondero, Donna Gabai, Emily Qian, Claire Shin          */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
#include <assert.h>
#include <stddef.h>

/*--------------------------------------------------------------------*/
/* Modify *puiDest in place,
   setting uiNumBits starting at uiDestStartBit (where 0 indicates
   the least significant bit) with bits taken from uiSrc,
   starting at uiSrcStartBit.
   uiSrcStartBit indicates the rightmost bit in the field.
   setField sets the appropriate bits in *puiDest to 1.
   setField never unsets any bits in *puiDest.                        */
/*--------------------------------------------------------------------*/
static void setField(unsigned int uiSrc, unsigned int uiSrcStartBit,
                     unsigned int *puiDest, unsigned int uiDestStartBit,
                     unsigned int uiNumBits)
{
    unsigned int mask, srcBits;

    mask = (1U << uiNumBits) - 1;
    srcBits = (uiSrc >> uiSrcStartBit) & mask;

    *puiDest |= (srcBits << uiDestStartBit);
}

/*-------------------------------------------------------------------*/
/* Return the machine language equivalent of "mov reg, immed" where
   reg is a W register.

   Parameters:
      uiReg: the number of reg.  0 <= uiReg <= 31.
      iImmed: the immed value. -32768 <= iImmed <= 32767      */
/*-------------------------------------------------------------------*/
unsigned int MiniAssembler_mov(unsigned int uiReg, int iImmed)
{
   /* Base Instruction Code */
   unsigned int uiInstr = 0x52800000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);
   setField(iImmed, 0, &uiInstr, 5, 16);

   return uiInstr;
}


/*-------------------------------------------------------------------*/
/* Return the machine language equivalent of "adr reg, addr".

   Parameters:
      uiReg: the number of reg. 0 <= uiReg <= 31.
      ulAddr: the address denoted by addr.
      ulAddrOfThisInstr: the address of the adr instruction itself
                         (must be a multiple of 4).                  */
/*-------------------------------------------------------------------*/
unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x10000000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* displacement to be split into immlo and immhi and inserted */
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   setField(uiDisp, 0, &uiInstr, 29, 2);
   setField(uiDisp, 2, &uiInstr, 5, 19);

   return uiInstr;
}

/*-------------------------------------------------------------------*/
/* Return the machine language equivalent of "strb fromreg,[toreg]",
   where fromreg is a W register and toreg is a X register.

   Parameters:
      uiFromReg: the number of fromreg. 0 <= uiFromReg <= 31.
      uiToReg: the number of toreg.     0 <= uiToReg <= 31.          */
/*-------------------------------------------------------------------*/
unsigned int MiniAssembler_strb(unsigned int uiFromReg, unsigned int uiToReg)
{
    unsigned int uiInstr = 0x39000000;

    setField(uiFromReg, 0, &uiInstr, 0, 5);
    
    setField(uiToReg, 0, &uiInstr, 5, 5);

    return uiInstr;
}


/*-------------------------------------------------------------------*/
/* Return the machine language equivalent of "b addr".

   Parameters:
      ulAddr: the address denoted by addr, that is, the address to
         which the branch should occur (must be a multiple of 4).
      ulAddrOfThisInstr: the address of the b instruction itself
         (must be a multiple of 4).                                  */
/*-------------------------------------------------------------------*/
unsigned int MiniAssembler_b(unsigned long ulAddr, unsigned long ulAddrOfThisInstr)
{
    unsigned int uiInstr = 0x14000000;
    unsigned int uiDisp = (unsigned int)((ulAddr - ulAddrOfThisInstr) / 4);

    setField(uiDisp, 0, &uiInstr, 0, 26);

    return uiInstr;
}

/*-------------------------------------------------------------------*/
/* Return the machine language equivalent of "bl addr", 
   where "bl" stands for "branch with link". This instruction branches 
   to the given address and saves the return address in register X30.

   Parameters:
      ulAddr: the address denoted by addr, that is, the address to
         which the branch should occur (must be a multiple of 4).
      ulAddrOfThisInstr: the address of the bl instruction itself
         (must be a multiple of 4).                                  */ 
/*-------------------------------------------------------------------*/
unsigned int MiniAssembler_bl(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x94000000;

   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   /* register to be inserted in instruction */
   setField(uiDisp, 2, &uiInstr, 0, 26);

   return uiInstr;

}