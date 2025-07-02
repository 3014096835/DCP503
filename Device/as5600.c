#include 	"as5600.h"
#include 	"i2c.h"
#include 	"bsp.h"
#include  "BoardVariable.h"

u8  buf[2] = {0};	       //��ű���;

u32 tempX0 = 0;			   //��ʼ�Ƕ�ԭʼ���ֵ
u32 tempX1 = 0;		       //�ϴνǶ�ԭʼ���ֵ
u32 tempX2 = 0;		       //��νǶ�ԭʼ���ֵ

u32 tempY0 = 0;			   //��ʼ�Ƕ�ԭʼ���ֵ
u32 tempY1 = 0;		       //�ϴνǶ�ԭʼ���ֵ
u32 tempY2 = 0;		       //��νǶ�ԭʼ���ֵ

void AS5600_Init ( void ) 
{
    IIC_Init();
}

u16 AS5600_Read_Len ( u8 addr, u8 reg, u8 len, u8 *buf ) 
{
    IIC_Start();
    IIC_Send_Byte ( ( addr << 1 ) | Write_Bit );

    if ( IIC_Wait_Ack() ) 
		{
        IIC_Stop();
        return 1;
    }

    IIC_Send_Byte ( reg );
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte ( ( addr << 1 ) | Read_Bit ); // ����������ַ + ������
    IIC_Wait_Ack(); // �ȴ�Ӧ��

    while ( len ) 
		{
        if ( len == 1 ) 
				{
            *buf = IIC_Read_Byte ( 0 ); // �����ݣ�����nACK
        } else 
				{
            *buf = IIC_Read_Byte ( 1 ) & 0x000f; // �����ݣ�����ACK ԭʼ1f,��Ϊ0f
        }
        len--;
        buf++;
    }

    IIC_Stop();
    return 0;
}


void Get_Ini_Val(void)//��ó�ʼ�Ƕ�
{
	u8 i = 0;
	u8 j = 0;
	u32 transfer = 0;
	unsigned char tran_num=0;
	unsigned char error=0;
	unsigned char Sflag=0;
	
	for(j=0;j<2;j++)
	{
		transfer=0;
		tran_num=0;
		error=0;
		IIC_Select=j;
	
		for ( i = 0; i < 10; i++ ) 
		{ // �տ�ʼ���ݿ��ܲ��ȶ���ֱ�Ӷ���
			Sflag = AS5600_Read_Len ( Slave_Addr, Angle_Hight_Register_Addr, 2, buf );
			error += Sflag;
			Delay_us ( 5 );
		}
		if(error<5)
		{
			for ( i = 0; i < 10; i++ ) 
			{ // ����˲�
				Sflag = AS5600_Read_Len ( Slave_Addr, Angle_Hight_Register_Addr, 2, buf );
				error += Sflag;
				if(Sflag==0)
				{
					transfer += ( ( buf[0] << 8 ) | buf[1] );
					tran_num++;
				}
				Delay_us ( 5 );
			}
		}
		if(error<5)
		{
			if(IIC_Select==0)
			{
				//CodeX.Error=NoErr;
				tempX0 = transfer / tran_num;
				tempX2 = tempX0;//��temp2��ʼ��Ϊ��ʼֵ
				tempX1 = tempX0;//��temp1��ʼ��Ϊ��ʼֵ
				CodeX.RealRead = tempX2;
			}
			else
			{
				//CodeY.Error=NoErr;
				tempY0 = transfer / tran_num;
				tempY2 = tempY0;//��temp2��ʼ��Ϊ��ʼֵ
				tempY1 = tempY0;//��temp1��ʼ��Ϊ��ʼֵ
				CodeY.RealRead = tempY2;
			}
		}
		else
		{
			if(IIC_Select==0)	{	CodeX.Error=Err;	}
			else		{	CodeX.Error=Err;	}
		}
	}
}

void Get_Num_sign(unsigned char Chx) //��ȡ��ǰ�Ƕ�ֵ����;
{
	unsigned int i;
	u32 transfer = 0;
	unsigned char tran_num=0;
	unsigned char error=0;
	unsigned char Sflag=0;
	
	IIC_Select = Chx; // ѡ�����ݻ�ȡͨ��;
	
	for ( i = 0; i < 5; i++ ) 
	{ // �տ�ʼ���ݿ��ܲ��ȶ���ֱ�Ӷ���
		Sflag = AS5600_Read_Len ( Slave_Addr, Angle_Hight_Register_Addr, 2, buf );
		error+=Sflag;
		Delay_us ( 5 );
	}
	if(error<5)
	{
		error=0;
		for ( i = 0; i < 5; i++ ) 
		{ // ����˲�
			Sflag = AS5600_Read_Len ( Slave_Addr, Angle_Hight_Register_Addr, 2, buf );
			error+=Sflag;
			if(Sflag==0)
			{
				transfer += ( ( buf[0] << 8 ) | buf[1] );
				tran_num++;
			}
			Delay_us ( 5 );
		}
	}
	
	if(error<5)
	{
		if(IIC_Select==0)
		{
			tempX2 = transfer/tran_num;
			CodeX.RealRead = tempX2;
			//CodeX.Error=NoErr;
			
			CodeX.AddRead = tempX2 - tempX0;
			
// 			if(tempX2 >= tempX0) //��ת
// 			{	CodeX.AddRead = tempX2 - tempX0;	}//dir = 0;
// 			else    			//��ת
// 			{	CodeX.AddRead = tempX0 - tempX2;	}//dir = 1;
		}
		else
		{
			tempY2 = transfer/tran_num;
			CodeY.RealRead = tempY2;
			//CodeY.Error=NoErr;
			
			CodeY.AddRead = tempY2 - tempY0;
			
// 			if(tempY2 >= tempY0) //��ת
// 			{	CodeY.AddRead = tempY2 - tempY0;	}//dir = 0;
// 			else    			//��ת
// 			{	CodeY.AddRead = tempY0 - tempY2;	}//dir = 1;
		}
	}
	else
	{
		if(IIC_Select==0)
		{
			CodeX.Error=Err;
		}
		else
		{
			CodeY.Error=Err;
		}
	}
}


