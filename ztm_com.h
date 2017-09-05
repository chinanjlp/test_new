#ifndef _COM_H_
#define _COM_H_


void SCI_Comm_ztm(void);
void Timer_cnt_increase(void);
void Timer_sampling_increase(void);
#if 0
/* SCI communication */
#define FMSTR_DISABLE           0x00   /* FreeMASTER is enabled */
#define FMSTR_USE_SCI           0x01   /* Use SCI interface */
#define FMSTR_SCI_BASE          0xE080 /* SCI Base address is same as SCI Baud Rate register address */
#define FMSTR_USE_JTAG          0x00   /* Disable JTAG interface */
#define FMSTR_USE_MSCAN         0x00   /* Disables MSCAN interface */
#define FMSTR_USE_FLEXCAN       0x00   /* Disables FLEXCAN interface */





/****************************************************************************************
* SCI module constants
*****************************************************************************************/

/* SCI module registers */
#define FMSTR_SCIBR_OFFSET 0U
#define FMSTR_SCICR_OFFSET 1U
#define FMSTR_SCISR_OFFSET 3U
#define FMSTR_SCIDR_OFFSET 4U

/* SCI Control Register bits */
#define FMSTR_SCICR_LOOP      0x8000U
#define FMSTR_SCICR_SWAI      0x4000U
#define FMSTR_SCICR_RSRC      0x2000U
#define FMSTR_SCICR_M         0x1000U
#define FMSTR_SCICR_WAKE      0x0800U
#define FMSTR_SCICR_POL       0x0400U
#define FMSTR_SCICR_PE        0x0200U
#define FMSTR_SCICR_PT        0x0100U
#define FMSTR_SCICR_TEIE      0x0080U
#define FMSTR_SCICR_TIIE      0x0040U
#define FMSTR_SCICR_RFIE      0x0020U
#define FMSTR_SCICR_REIE      0x0010U
#define FMSTR_SCICR_TE        0x0008U
#define FMSTR_SCICR_RE        0x0004U
#define FMSTR_SCICR_RWU       0x0002U
#define FMSTR_SCICR_SBK       0x0001U

/* SCI Status registers bits */
#define FMSTR_SCISR_TDRE      0x8000U
#define FMSTR_SCISR_TIDLE     0x4000U
#define FMSTR_SCISR_RDRF      0x2000U
#define FMSTR_SCISR_RIDLE     0x1000U
#define FMSTR_SCISR_OR        0x0800U
#define FMSTR_SCISR_NF        0x0400U
#define FMSTR_SCISR_FE        0x0200U
#define FMSTR_SCISR_PF        0x0100U
#define FMSTR_SCISR_RAF       0x0001U

/*******************************************************************************************
* SCI access macros
*****************************************************************************************/

/* transmitter enable/disable */
/*lint -emacro(923,FMSTR_SCI_TE, FMSTR_SCI_TD) : casting long to pointer */
#define FMSTR_SCI_TE() FMSTR_SETBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_TE)
#define FMSTR_SCI_TD() FMSTR_CLRBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_TE)

/* receiver enable/disable */
/*lint -emacro(923,FMSTR_SCI_RE, FMSTR_SCI_RD) : casting long to pointer */
#define FMSTR_SCI_RE() FMSTR_SETBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_RE)
#define FMSTR_SCI_RD() FMSTR_CLRBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_RE)

#define FMSTR_SCI_TE_RE() FMSTR_SETBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_RE | FMSTR_SCICR_TE)

/* Transmitter-empty interrupt enable/disable : casting long to pointer */
/*lint -emacro(923,FMSTR_SCI_ETXI, FMSTR_SCI_DTXI) */
#define FMSTR_SCI_ETXI() FMSTR_SETBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_TEIE)
#define FMSTR_SCI_DTXI() FMSTR_CLRBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_TEIE)

/* Receiver-full interrupt enable/disable : casting long to pointer */
/*lint -emacro(923,FMSTR_SCI_ERXI, FMSTR_SCI_DRXI) */
#define FMSTR_SCI_ERXI() FMSTR_SETBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_RFIE)
#define FMSTR_SCI_DRXI() FMSTR_CLRBIT(FMSTR_SCI_BASE, FMSTR_SCICR_OFFSET, FMSTR_SCICR_RFIE)

/* Tranmsit character */
/*lint -emacro(923,FMSTR_SCI_PUTCHAR) : casting long to pointer */
#define FMSTR_SCI_PUTCHAR(ch) FMSTR_SETREG(FMSTR_SCI_BASE, FMSTR_SCIDR_OFFSET, ch)

/* Get received character */
/*lint -emacro(923,FMSTR_SCI_GETCHAR) : casting long to pointer */
#define FMSTR_SCI_GETCHAR() FMSTR_GETREG(FMSTR_SCI_BASE, FMSTR_SCIDR_OFFSET)

/* read status register */
/*lint -emacro(923,FMSTR_SCI_GETSR) : casting long to pointer */
#define FMSTR_SCI_GETSR()   FMSTR_GETREG(FMSTR_SCI_BASE, FMSTR_SCISR_OFFSET)

/* read & clear status register (clears error status bits only) */
/*lint -emacro(923,FMSTR_SCI_RDCLRSR) : casting long to pointer */
#define FMSTR_SCI_RDCLRSR() FMSTR_SETREG(FMSTR_SCI_BASE, FMSTR_SCISR_OFFSET, FMSTR_GETREG(FMSTR_SCI_BASE, FMSTR_SCISR_OFFSET))

#endif

#endif
