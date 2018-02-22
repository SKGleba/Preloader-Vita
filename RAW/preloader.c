// Preloader base
// UberLight version, only psTV and/or plugin loader (on vita it pratically wont work)
// reported problems with latest SDK
// Spoof by Zecoxao , mod by SKGleba

/*
 * mmu_dump.c - Bare metal ARMv7 translation table dumper
 * Copyright 2014 Yifan Lu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/io/fcntl.h>

static int afe;

void _start() __attribute__ ((weak, alias ("mstrt")));

static unsigned int p2v(unsigned int pa)
{
    unsigned int va;
    unsigned int vd;
    unsigned int pd;
    unsigned int i;

    va = 0;
    for (i = 0; i < 0x100000; i++)
    {
        vd = i << 12;
        __asm__("mcr p15,0,%1,c7,c8,0\n\t"
                "mrc p15,0,%0,c7,c4,0\n\t" : "=r" (pd) : "r" (vd));
        if ((pa & 0xFFFFF000) == (pd & 0xFFFFF000))
        {
            va = vd + (pa & 0xFFF);
            break;
        }
    }
    return va;
}

static void gperms(int ap2, int ap1, int *ur, int *uw, int *pr, int *pw)
{

    if (afe)
    {
        *ur = ap1 > 1;
        *uw = !ap2 && ap1 > 1;
        *pr = 1;
        *pw = !ap2 && ap1 < 2;
    }
    else
    {
        *pw = (!ap2 && ap1);
        *pr = *pw || ap1;
        *ur = ap1 > 1;
        *uw = !ap2 && ap1 == 3;
    }
}


unsigned char mgc[0x8] = {
	0xA5, 
    0x9D, 
    0xCE, 
    0xAB, 
    0x00, 
    0x01, 
    0x01
};


unsigned char dcd[0x8] = {
// no effect
	0x00, 
    0x00, 
// Must be 0x01, otherwise-chinese shit
    0x01, 
/*
 *00 -- no activation,testkit+devkit, i think prototype
 *01 -- Devkit , battery failed
 *02 -- Testkit , exp
 *03 -- FalsePositive/no testkit func but health warn
 *04 -- same
 *05 -- Looks like retail
 *06 -- same as 3
 *07 -- same as 5
 *08 -- same as 3
 *09 -- same as 5
 *10+ -- chinese shit, C1-13819-2
 */
    0x01, 
//no effect
    0x00, 
    0x10, 
    0x0C, 
    0x12
};

static int dpag(unsigned int vd, unsigned int etr)
{
    int ap2;
    int ap1;
    int pr;
    int pw;
    int ur;
    int uw;
    unsigned int pd;

    if ((etr & 0x3) == 0x1)
    {
        ap2 = etr & 0x200;
        ap1 = (etr >> 4) & 3;
        gperms(ap2, ap1, &ur, &uw, &pr, &pw);
        pd = etr & 0xFFFF0000;
	if( pd >= 0x40201000 && pd < 0x5FD00000 && pd != 0x44C20000 && pd != 0x44C30000 && pd != 0x443C0000){
		if( (pd-0x44300000) <= (0x44400000-0x44300000) ){

			}else if ( (pd-0x45020000) <= (0x45030000-0x45020000) ){

			}else{
				
				
				
				
				int ntr = 0;
				for (ntr = 0; ntr < 0x1000; ntr ++){
					if(memcmp((void*)vd + ntr, mgc, 7) == 0 && ntr == 0x5B0){
						ntr = ntr + 4;
						memcpy((void*) vd + ntr, dcd, 8);
						return 0;
					}
				}
            }
		}
    }
    else if ((etr & 0x2)) 
    {
        ap2 = etr & 0x200;
        ap1 = (etr >> 4) & 3;
        gperms(ap2, ap1, &ur, &uw, &pr, &pw);
        pd = etr & 0xFFFFF000;

        if( pd >= 0x40201000 && pd < 0x5FD00000){
			if( (pd-0x47D80000) <= (0x47D90000-0x47D80000) ){

			}else if ( (pd-0x44C09000) <= (0x44C1A000-0x44C09000) ){

			}else if ( (pd-0x44300000) <= (0x44400000-0x44300000) ){

			}else if ( (pd-0x45009000) <= (0x45019000-0x45009000) ){

			}else if ( (pd-0x44700000) <= (0x44710000-0x44700000) ){

			}else{
				
				
				
				
				int ntr = 0;
				for (ntr = 0; ntr < 0x1000; ntr ++){
					if(memcmp((void*)vd + ntr, mgc, 7) == 0 && ntr  == 0x5B0){
						ntr = ntr + 4;
						memcpy((void*) vd + ntr, dcd, 8);
						return 0;
					}
				}
			}
            
        }
    }

	return 1;
}

static int dsect(unsigned int vd, unsigned int etr)
{
    int ss;
    int ap1;
    int ap2;
    int pr;
    int pw;
    int ur;
    int uw;
    unsigned int pd;
    unsigned int i;
    unsigned int *tbl;
    

    if ((etr & 0x3) == 2)
    {
        ss = etr & 0x40000;
        ap2 = etr & 0x8000;
        ap1 = (etr >> 10) & 3;
        gperms(ap2, ap1, &ur, &uw, &pr, &pw);
        pd = ss ? etr & 0xFF000000 : etr & 0xFFF00000;

    }
    else if ((etr & 0x3) == 1)
    {
        pd = etr & 0xFFFFFC00;
        tbl = (unsigned int *)p2v(pd);
        for (i = 0; i < 0x100; i++)
        {
            int ret = dpag(vd+(i<<12), tbl[i]);
			if (ret == 0) {
				return 0;
			}
        }
    }

	return 1;
}

int mdmp(void)
{
    unsigned int ttbr[2];
    int ttbcr;
    int n;
    unsigned int i = 0;
    
    unsigned int *ttb_vd[2];
    unsigned int etr;
	
    __asm__("mrc p15,0,%0,c2,c0,0" : "=r" (ttbr[0]));
    __asm__("mrc p15,0,%0,c2,c0,1" : "=r" (ttbr[1]));
    __asm__("mrc p15,0,%0,c2,c0,2" : "=r" (ttbcr));

    n = ttbcr & 0x7;
    ttbr[0] &= (unsigned int)((int)0x80000000 >> (31 - 14 + 1 - n));
    ttbr[1] &= 0xFFFFC000;

    ttb_vd[0] = (unsigned int *)p2v(ttbr[0]);
    ttb_vd[1] = (unsigned int *)p2v(ttbr[1]);

    for (i = 0; i < (1 << 12 - n); i++)
    {
        etr = ttb_vd[0][i];
        int ret = dsect(i<<20, etr);
		if(ret == 0){
			return 0;
		}
    }

    if (n)
    {
        for (i = ((~0xEFFF & 0xFFFF) >> n); i < 0x1000; i++)
        {
            etr = ttb_vd[1][i];
            int ret = dsect(i<<20, etr);
			if(ret == 0){
				return 0;
			}
        }
    }
    return 1;
}

int mstrt(SceSize argc, const void *args)
{
 // add mdmp(); to create a fully working light spoofer.
    return SCE_KERNEL_START_SUCCESS;
}

int mstp(SceSize argc, const void *args)
{
	return SCE_KERNEL_STOP_SUCCESS;
}

