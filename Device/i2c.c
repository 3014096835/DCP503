#include "i2c.h"

unsigned char IIC_Select=0;

void Delay_us(unsigned int Count)
{
	unsigned int is;
	while(Count--)
	{
		is=9;
		while(is--);
	}
}

void IIC_Init ( void )
{
    GPIO_InitTypeDef GPIO_Init_Structure;
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );
    GPIO_Init_Structure.GPIO_Pin = GPIO_SCL01 | GPIO_SDA01 | GPIO_SCL02 | GPIO_SDA02;
    GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( GPIOA, &GPIO_Init_Structure );
    GPIO_SetBits ( GPIOA, GPIO_SCL01 | GPIO_SDA01 | GPIO_SCL02 | GPIO_SDA02);
}

void SDA_IN ( void ) 
{
	if(IIC_Select==0)
	{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_SDA01;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );
	}
	else
	{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_SDA02;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );
	}
}

void SDA_OUT ( void ) 
{
	if(IIC_Select==0)
	{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_SDA01;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );
	}
	else
	{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_SDA02;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );
	}
}


void IIC_Start ( void ) 
{
	if(IIC_Select==0)
	{
    SDA_OUT();
    IIC_SDA01_H;
    IIC_SCL01_H;
    Delay_us ( 5 );
    IIC_SDA01_L;
    Delay_us ( 5 );
    IIC_SCL01_L;
	}
	else
	{
    SDA_OUT();
    IIC_SDA02_H;
    IIC_SCL02_H;
    Delay_us ( 5 );
    IIC_SDA02_L;
    Delay_us ( 5 );
    IIC_SCL02_L;
	}
}

void IIC_Stop ( void ) 
{
	if(IIC_Select==0)
	{
    SDA_OUT();
    IIC_SDA01_L;
    IIC_SCL01_H;
    Delay_us ( 5 );
    IIC_SDA01_H;
    Delay_us ( 5 );
	}
	else
	{
    SDA_OUT();
    IIC_SDA02_L;
    IIC_SCL02_H;
    Delay_us ( 5 );
    IIC_SDA02_H;
    Delay_us ( 5 );
	}
}

void IIC_Ack ( void ) 
{
	if(IIC_Select==0)
	{
    IIC_SCL01_L;
    SDA_OUT();
    IIC_SDA01_L;
    Delay_us ( 5 );
    IIC_SCL01_H;
    Delay_us ( 5 );
    IIC_SCL01_L;
	}
	else
	{
    IIC_SCL02_L;
    SDA_OUT();
    IIC_SDA02_L;
    Delay_us ( 5 );
    IIC_SCL02_H;
    Delay_us ( 5 );
    IIC_SCL02_L;
	}
}

void IIC_NAck ( void ) 
{
	if(IIC_Select==0)
	{
    IIC_SCL01_L;
    SDA_OUT();
    IIC_SDA01_H;
    Delay_us ( 5 );
    IIC_SCL01_H;
    Delay_us ( 5 );
    IIC_SCL01_L;
	}
	else
	{
    IIC_SCL02_L;
    SDA_OUT();
    IIC_SDA02_H;
    Delay_us ( 5 );
    IIC_SCL02_H;
    Delay_us ( 5 );
    IIC_SCL02_L;
	}
}

u8 IIC_Wait_Ack ( void ) 
{
    u8 ucErrTime = 0;
	
	if(IIC_Select==0)
	{
    IIC_SDA01_H;
    Delay_us ( 5 );
    SDA_IN();
    IIC_SCL01_H;
    Delay_us ( 5 );

    while ( READ_SDA01 ) 
		{
        ucErrTime++;
        if ( ucErrTime > 250 ) 
				{   return 1;   }
    }
    IIC_SCL01_L;
    return 0;
	}
	else
	{
    IIC_SDA02_H;
    Delay_us ( 5 );
    SDA_IN();
    IIC_SCL02_H;
    Delay_us ( 5 );

    while ( READ_SDA02 ) 
		{
        ucErrTime++;
        if ( ucErrTime > 250 ) 
				{   return 1;   }
    }
    IIC_SCL02_L;
    return 0;
	}
}


void IIC_Send_Byte ( u8 txd ) 
{
    u8 t;
	
	if(IIC_Select==0)
	{
    SDA_OUT();
    IIC_SCL01_L;

    for ( t = 0; t < 8; t++ ) 
		{
        if ( ( txd & 0x80 ) >> 7 ) {
            IIC_SDA01_H;
        } else {
            IIC_SDA01_L;
        }

        txd <<= 1;
        Delay_us ( 5 );
        IIC_SCL01_H;
        Delay_us ( 5 );
        IIC_SCL01_L;
        Delay_us ( 5 );
    }
	}
	else
	{
    SDA_OUT();
    IIC_SCL02_L;

    for ( t = 0; t < 8; t++ ) 
		{
        if ( ( txd & 0x80 ) >> 7 ) {
            IIC_SDA02_H;
        } else {
            IIC_SDA02_L;
        }

        txd <<= 1;
        Delay_us ( 5 );
        IIC_SCL02_H;
        Delay_us ( 5 );
        IIC_SCL02_L;
        Delay_us ( 5 );
    }
	}
}

u8 IIC_Read_Byte ( u8 ack ) 
{
    unsigned char i, receive = 0;
	
		if(IIC_Select==0)
		{
			SDA_IN();
			for ( i = 0; i < 8; i++ ) 
			{
					IIC_SCL01_L;
					Delay_us ( 5 );
					IIC_SCL01_H;
					receive <<= 1;
					
					if ( READ_SDA01 ) {  receive++;  }
					Delay_us ( 5 );
			}
			if ( !ack ) {   IIC_NAck();  }
			else {  IIC_Ack();   }
			return receive;
		}
		else
		{
			SDA_IN();
			for ( i = 0; i < 8; i++ ) 
			{
					IIC_SCL02_L;
					Delay_us ( 5 );
					IIC_SCL02_H;
					receive <<= 1;
					
					if ( READ_SDA02 ) {  receive++;  }
					Delay_us ( 5 );
			}
			if ( !ack ) {   IIC_NAck();  }
			else {  IIC_Ack();   }
			return receive;
		}
		

}
