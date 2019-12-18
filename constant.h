//ˮ�ǣ����ǣ����򣬻��ǣ�ľ�ǣ����ǣ������ǣ�������
#define R_RATIO (4.0e5f)
#define D_RATIO (8.0e5f)

//��������
//#define G 

//�뾶(m)
#define R_SUN     ((6.96e8f)/R_RATIO) 
#define R_MERCURY ((4.878e6f)/2.0f/R_RATIO)//ˮ��
#define R_EARTH ((1.2756e7f)/2.0f/R_RATIO)//����
#define R_MOON  ((3.476e6f)/2.0f/R_RATIO)//����
#define R_VENUS ((6.0518e6f)/R_RATIO)//����
#define R_MARS  ((3.397e6f)/R_RATIO)//����
#define R_JUPITER ((7.1492e7f)/R_RATIO)//ľ��
#define R_SATURN ((6.0268e7f)/R_RATIO)//����
#define R_URANUS ((2.5559e7f)/R_RATIO)//������
#define R_NEPTUNE ((2.4764e7f)/R_RATIO)//������

//��̫������(m)
#define DIS_SUN_MERCURY ((5.971e10)/D_RATIO)
#define DIS_SUN_VENUS ((1.082e11)/D_RATIO)
#define DIS_SUN_EARTH ((1.495978875e11f)/D_RATIO) 
#define DIS_EARTH_MOON ((3.84403e8f)/D_RATIO)
#define DIS_SUN_MARS ((2.2794e11f)/D_RATIO)
#define DIS_SUN_JUPITER ((7.7833e11f)/D_RATIO)
#define DIS_SUN_SATURN ((1.42694e12f)/D_RATIO)
#define DIS_SUN_URANUS ((2.87099e12f)/D_RATIO)
#define DIS_SUN_NEPTUNE ((4.49707e12f)/D_RATIO)

//������
#define TA_MERCURY (-7.00487f*D3DX_PI/180.0f)
#define TA_VENUS (-3.395f*D3DX_PI/180.0f)
#define TA_EARTH (-0.0f*D3DX_PI/180.0f)
#define TA_MARS  (-1.85061f*D3DX_PI/180.0f)
#define TA_JUPITER (-1.3053f*D3DX_PI/180.0f)
#define TA_SATURN (-2.49f*D3DX_PI/180.0f)
#define TA_URANUS (-0.77f*D3DX_PI/180.0f)
#define TA_NEPTUNE (-1.769f*D3DX_PI/180.0f)
#define TA_MOON (-8.98041015189461e-2f)

//��ת�����
#define AA_MERCURY (-0.1f*D3DX_PI/180.0f)
#define AA_VENUS (-177.0f*D3DX_PI/180.0f)
#define AA_EARTH (-23.0f*D3DX_PI/180.0f)
#define AA_MARS  (-25.0f*D3DX_PI/180.0f)
#define AA_JUPITER (-3.0f*D3DX_PI/180.0f)
#define AA_SATURN (-27.0f*D3DX_PI/180.0f)
#define AA_URANUS (-98.0f*D3DX_PI/180.0f)
#define AA_NEPTUNE (-30.0f*D3DX_PI/180.0f)
#define AA_MOON (0.0f)

//��ת���ڣ�s��
#define AT_SUN (2.192832e6f)// = 25.38��
#define AT_MERCURY (5067360.f)
#define AT_VENUS (20996064.f)
#define AT_EARTH (86164.0f)//23H56MIN4S
#define AT_MARS (88646.4f)
#define AT_JUPITER (35424.f)
#define AT_SATURN (36806.4f)
#define AT_URANUS (55814.4f)
#define AT_NEPTUNE (56851.2f)


//��ת���ڣ�s�� revolution
#define RT_MERCURY (7600530.24f)//87.9691��
#define RT_VENUS   (19414080.0f)//224.7��
#define RT_EARTH   (31556926.0f)//365d5h48min46s
#define RT_MARS    (59355072.0f)//686.98��
#define RT_JUPITER (374346144.0f)//4332.71��
#define RT_SATURN  (929620800.0f)//10759.5��
#define RT_URANUS  (2651184000.0f)//30685��
#define RT_NEPTUNE (5200581417.984f)//60191.91456��
#define RT_MOON    (2360591.559f)//27d7h43min11.559s

//�ư׽���
//#define T_LINEAXIS (586958400.0f)//�ư׽�������(s) = 6793.5d
#define TLA_MOON (586958400.0f)
//����(kg)
#define M_SUN     (1.9891e30f)	
#define M_MERCURY (3.3022e23f)
#define M_EARTH   (5.965e24f)
#define M_MOON    (7.349e22f)

