

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_protocol.h"
#include "ztm_com.h"

/* in older versions, the SCI flags were tested directly, new version is more general,
 * but it remains backward compatible. If SCI flag test macros are not defined, use
 * the equivalent of the old functionality. */
#ifndef FMSTR_SCI_TXFULL
#define FMSTR_SCI_TXFULL(wSciSR)  (!((wSciSR) & FMSTR_SCISR_TDRE))
#endif
#ifndef FMSTR_SCI_TXEMPTY
#define FMSTR_SCI_TXEMPTY(wSciSR) ((wSciSR) & FMSTR_SCISR_TC)
#endif
#ifndef FMSTR_SCI_RXREADY
    /* when overrun flag is defined, use it also to detect incoming character (and to 
     * make sure the OR flag is cleared in modules where RDRF and OR are independent */ 
    #ifdef FMSTR_SCISR_OR
    #define FMSTR_SCI_RXREADY(wSciSR) ((wSciSR) & (FMSTR_SCISR_RDRF | FMSTR_SCISR_OR))
    #else
    #define FMSTR_SCI_RXREADY(wSciSR) ((wSciSR) & (FMSTR_SCISR_RDRF))
    #endif
#endif





#if 1
//add by Lvpeng for ZTM

//timer freq: 16K. every 62.5us be called, 19200 n,8,1    1.5byte around 781us  (13 times)
#define LENGTH_1_5_BYTE  (13)
//20ms/62.5us = 320
#define DELAY_SAMPLING  320
#define SAMPLING_DURATION 1000  //20S/200ms = 1000


unsigned char Com_timer_status = 0;

unsigned short Cnt_timer = 0;
unsigned short Cnt_sampling_timer = 0;


//unsigned char Rx_byte = 0U;
//unsigned char Tx_byte = 0U;



unsigned char flg_speed_diff_output_start = 0;
unsigned char flg_torque_output_start  = 0;

static void Enable_Tx(void);
static void Enable_Rx(void);

static void Refresh_Rec_timer(void);
static void Disable_timer(void);
static void Enable_timer(void);

static void Refresh_sampling_timer(void);


static unsigned char Get_timer_status(void);
static unsigned char Check_Rec_timeout(void);
static unsigned char Check_sampling_timeout(void);

//extern Set_speed(unsigned short speed_value);

#if 0
extern void Set_speed(unsigned short);
extern unsigned short Get_speed(void);
extern unsigned short Get_speed_diff(unsigned short refer_speed,unsigned short offset_speed);
extern unsigned short Get_torque(void);
#endif




void Parse_frame(unsigned char *pBuffer_rec, unsigned char Len_rec,
	                         unsigned char *pBuffer_send, unsigned char *pLen_send);

void Enable_Tx(void)
{
	FMSTR_SCI_RD();
	FMSTR_SCI_TE();	
}

void Enable_Rx(void)
{
	 FMSTR_SCI_TD();
	 FMSTR_SCI_RE();	
}


void Disable_timer(void)
{
	Com_timer_status = 0;
}
 
void Enable_timer(void)
{
	Com_timer_status = 1;
}

unsigned char Get_timer_status(void)
{
	return Com_timer_status;
}



//COM Timer
//every 62.5us be called, 19200 n,8,1    1.5byte around 781us  (13 times)
void Timer_cnt_increase(void)
{	
	Cnt_timer++;
}

void Refresh_Rec_timer(void)
{
	Cnt_timer = 0;
}

unsigned char Check_Rec_timeout(void)
{
	
	if(Cnt_timer > LENGTH_1_5_BYTE)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}



// OOB output Timer
void Timer_sampling_increase(void)
{	
	Cnt_sampling_timer++;
}

void Refresh_sampling_timer(void)
{
	Cnt_sampling_timer = 0;
}

unsigned char Check_sampling_timeout(void)
{
	
	if(Cnt_sampling_timer > DELAY_SAMPLING)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}






/*****/
// cmd1: 0x01 byte(H) byte(L)   set speed
// cmd2: 0x02  get actual speed
// cmd3: 0x03  oob start ,send actual speed period.
/****/
void Parse_frame(unsigned char *pBuffer_rec, unsigned char Len_rec,
	                         unsigned char *pBuffer_send, unsigned char *pLen_send)
{
	unsigned short tempH = 0;
	unsigned short tempL = 0;
	
	if((0x01 == *(pBuffer_rec+0))&&(3 == Len_rec))
	{
	
		//set speed;
		tempH = *(pBuffer_rec+1);
		tempH = tempH*256;
		tempL =  *(pBuffer_rec+2);
		
		//Set_speed(tempH+tempL);
		
		 *(pBuffer_send+0) =  *(pBuffer_rec+0);
		*(pBuffer_send+1) =  *(pBuffer_rec+1);
		*(pBuffer_send+2) =  *(pBuffer_rec+2);
		*pLen_send = 3;
	}
	else if((0x02 == *(pBuffer_rec+0))&&(1 == Len_rec))
	{
		//get speed;
	
		 *(pBuffer_send+0) =  *(pBuffer_rec+0);
		*(pBuffer_send+1) =  tempH;
		*(pBuffer_send+2) =  tempL;
		*pLen_send = 3;
	}
	else if((0x03 == *(pBuffer_rec+0))&&(1 == Len_rec))
	{
		flg_speed_diff_output_start = 1;
		*pLen_send = 0;
		//atctual speed difference for 20s
	}
	else if((0x04 == *(pBuffer_rec+0))&&(1 == Len_rec))
	{
		flg_torque_output_start = 1;
		*pLen_send = 0;
		//atctual torque  for 20s
	}

	//Buff_Tx[0] =0x55;
	//Buff_Tx[1] =0xaa;
	//Len_Tx = 2;
	
	//switch to transmit mode
	
	
}





void SCI_Comm_ztm(void)
{
	unsigned short Sci_reg_status;
	unsigned char nRxChar = 0U;
	static unsigned char Sci_tx_status = 0;
	static unsigned char Index_Rx = 0;
       static unsigned char Index_Tx = 0;
	static unsigned char Buff_Rx[20];
       static unsigned char Buff_Tx[20];
	static unsigned char Len_Rx = 0;
	static unsigned char Len_Tx = 0;
	
	/* read & clear status     */
	Sci_reg_status = FMSTR_SCI_RDCLRSR();
	 
	if((1 == Check_Rec_timeout())&&
	    (1 == Get_timer_status())
	   )
	{	
		Disable_timer();
		Parse_frame(Buff_Rx,Len_Rx, Buff_Tx, &Len_Tx);
		
		Len_Rx = 0;	// Clear rec status
		Index_Rx  = 0;
		Sci_tx_status = 1;
		Enable_Tx();
	}
	
	if(1 == Sci_tx_status)			//send response
	{
		
		if(!FMSTR_SCI_TXFULL(Sci_reg_status))		//able to receive a byte to buffer?
		{
			if(Len_Tx != Index_Tx)
			{
				nRxChar = Buff_Tx[Index_Tx++];
				FMSTR_SCI_PUTCHAR(nRxChar);	
			}
			else				//end transmit, switch to receive mode
			{	
				Len_Tx = 0;		//clear trans status
				Index_Tx = 0;
				Sci_tx_status  = 0;
				Enable_Rx();	
			}	
		} 
	}
	else							//receive command
	{
	   /* data byte received? */
	   if (FMSTR_SCI_RXREADY(Sci_reg_status))
	   {
		   Enable_timer();
		   Refresh_Rec_timer();		//refresh timer
		   
		   nRxChar = (FMSTR_BCHR) FMSTR_SCI_GETCHAR();
		   Buff_Rx[Index_Rx++] = nRxChar;
		   Len_Rx = Index_Rx;
	   }
	}
	 
}



#if 0

static unsigned short Cnt_20s=0;
static unsigned shrot Speed_diff;
//static unsigned shrot Speed_refer = 3000;

void Send_speed_diff(void)
{

	
	if(1 == flg_speed_diff_output_start)
	{
		if(1 == Check_sampling_timeout())
		{
			Refresh_sampling_timer();
			Speed_diff=Get_speed_diff(1000, 3000);		
			Buff_Tx[0] =unsigned short (Speed_diff>>8);
			Buff_Tx[1] =unsigned short (Speed_diff);
			Len_Tx = 2;

			Sci_tx_status = 1;
			Enable_Tx();
			
			Cnt_20s++;
			if(Cnt_20s == SAMPLING_DURATION)
			{
				Cnt_20s = 0;
				flg_speed_diff_output_start = 0;
			}
		}
	}
}







void Set_speed(unsigned short speed_value)
{
	
}

unsigned short Get_speed(void)
{

}

unsigned short Get_speed_diff(unsigned short refer_speed, unsigned short offset_speed)
{
	
}

unsigned short Get_torque(void)
{
	
}
#endif
//add end
#endif
