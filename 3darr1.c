#include <stdlib.h>
#include <stdio.h>
#define MAXTHREADS 1000000
#define THREADSTACK 6553600
#include <pthread.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int i; //pętla for
    //int a=1024;
    int a=447;
    int b=a*a*a*2-1; // 178629245 -> 170MB             *2147483648 INTMAX -> 2GB

    char *c1,*c2,*c3; //3 zakresy bo 3 tablice index tablicy to max int
    c1 = malloc (sizeof(char)*b);
    c2 = malloc (sizeof(char)*b);
    c3 = malloc (sizeof(char)*b);
    for (i=0;i<b;i++)
    {
	c1[i]=10;
	c2[i]=20;
	c3[i]=30;
    }
    if (i==b)
    {
	printf ("c1,c2,c3 arrays mem alloc 3 x %d bytes (3 x %dMB) done!\n",b,(b+1)/1024/1024);
	printf ("so you get cube 812 x 812 x 812 witch cells 8-bits each wide\n\n");
    }

    sleep(5);
    //int cube=1860; //6 GB to cube 1860x1860x1860 --> pierwiastek 3-go stopnia z 3*INTMAX
    int cube=810;  //bo łatwo się dzieli na 3, w sumie 512MB 3-wym. tablica char[][][]
    int za1=1-1;    //3 zakresy: podzielimy po współrzędnej z; z1---<c1>---z2-z3---<c2>---z4-z5---<c3>---z6
    int za2=270-1;  //0-619,620-1239,1240-1860
    int za3=271-1;
    int za4=540-1;
    int za5=541-1;
    int za6=810-1;
    int za7=811-a;
    int x1=-1;	//P1xyx
    int y1=-1;
    int z1=-1;
    int x2=-1;	//P2xyx
    int y2=-1;
    int z2=-1;
    int x3=-1;	//P3xyx
    int y3=-1;
    int z3=-1;
    int x4=-1;	//P4xyx
    int y4=-1;
    int z4=-1;
    int x5=-1;	//P5xyx
    int y5=-1;
    int z5=-1;
    int x6=-1;	//P6xyx
    int y6=-1;
    int z6=-1;
    int x7=-1;	//P7xyx
    int y7=-1;
    int z7=-1;
    int x8=-1;	//P8xyx
    int y8=-1;
    int z8=-1;
    int p1=0; // określamy czy punkt jest określony (p1=1) czy nie określony p1=0
    int p2=0;
    int p3=0;
    int p4=0;
    int p5=0; 
    int p6=0;
    int p7=0;
    int p8=0;
    int val1=-1; // wartości do zapisania w p1..p8
    int val2=-1;
    int val3=-1;
    int val4=-1;
    int val5=-1; // wartości do zapisania w p1..p8
    int val6=-1;
    int val7=-1;
    int val8=-1;
    int choice=0;
    int quit=0;
    while (quit==0)
    {
	printf("\e[1;1H\e[2J");
	if( x1<0 || x1>cube || y1<0 || y1>cube || z1<0 || z1>cube ) // sprawdzamy czy określono punky i pamiętamy zmienne p1..p8
	{
	    printf("P1: niekreślono wpółrzędnych\n");
	    p1=0;
	}
	else
	{
	    printf("P1: x1=%d\ty1=%d\tz1=%d\n",x1,y1,z1);
	    p1=1; 
	}
	if( x2<0 || x2>cube || y2<0 || y2>cube || z2<0 || z2>cube )
	{	
	    printf("P2: niekreślono wpółrzędnych\n");
	    p2=0;
	}
	else
	{
	    printf("P2: x2=%d\ty2=%d\tz2=%d\n",x2,y2,z2);
	    p2=1; 
	}
	if( x3<0 || x3>cube || y3<0 || y3>cube || z3<0 || z3>cube )
	{
	    printf("P3: niekreślono wpółrzędnych\n");
	    p3=0;
	}
	else
	{
	    printf("P3: x3=%d\ty3=%d\tz3=%d\n",x3,y3,z3);
	    p3=1; 
	}
	if( x4<0 || x4>cube || y4<0 || y4>cube || z4<0 || z4>cube )
	{
	    printf("P4: niekreślono wpółrzędnych\n");
	    p4=0;
	}
	else
	{
	    printf("P4: x4=%d\ty4=%d\tz4=%d\n",x4,y4,z4); 
	    p4=1;
	}

	if( x5<0 || x5>cube || y5<0 || y5>cube || z5<0 || z5>cube )
	{
	    printf("P5: niekreślono wpółrzędnych\n");
	    p5=0;
	}
	else
	{
	    printf("P5: x5=%d\ty5=%d\tz5=%d\n",x5,y5,z5); 
	    p5=1;
	}

	if( x6<0 || x6>cube || y6<0 || y6>cube || z6<0 || z6>cube )
	{
	    printf("P6: niekreślono wpółrzędnych\n");
	    p6=0;
	}
	else
	{
	    printf("P6: x6=%d\ty6=%d\tz6=%d\n",x6,y6,z6); 
	    p6=1;
	}

	if( x7<0 || x7>cube || y7<0 || y7>cube || z7<0 || z4>cube )
	{
	    printf("P7: niekreślono wpółrzędnych\n");
	    p7=0;
	}
	else
	{
	    printf("P7: x7=%d\ty7=%d\tz7=%d\n",x7,y7,z7); 
	    p7=1;
	}

	if( x8<0 || x8>cube || y8<0 || y8>cube || z8<0 || z8>cube )
	{
	    printf("P8: niekreślono wpółrzędnych\n");
	    p8=0;
	}
	else
	{
	    printf("P8: x8=%d\ty8=%d\tz8=%d\n",x8,y8,z8); 
	    p8=1;
	}
	if 
	(   
	    0<=val1 && val1<=255 && 0<=val2 && val2<=255 && 0<=val3 && val3<=255 && 0<=val4 && val4<=255 &&
	    0<=val5 && val5<=255 && 0<=val6 && val6<=255 && 0<=val7 && val7<=255 && 0<=val8 && val8<=255 
	)
	
	{
	    printf("podane wartości: val1=%d val2=%d val3=%d val4=%d \n",val1,val2,val3,val4);
	    printf("podane wartości: val5=%d val6=%d val7=%d val8=%d \n",val5,val6,val7,val8);
	}
	printf("Menu:\n");
	printf("1. podaj współrzędne wierzchołka P1 sześcianu - dolna ściana\n");
	printf("2. podaj współrzędne wierzchołka P2 sześcianu - dolna ściana\n");
	printf("3. podaj współrzędne wierzchołka P3 sześcianu - dolna ściana\n");
	printf("4. podaj współrzędne wierzchołka P4 sześcianu - dolna ściana\n");
	printf("5. podaj współrzędne wierzchołka P5 sześcianu - górna ściana\n");
	printf("6. podaj współrzędne wierzchołka P6 sześcianu - górna ściana\n");
	printf("7. podaj współrzędne wierzchołka P7 sześcianu - górna ściana\n");
	printf("8. podaj współrzędne wierzchołka P8 sześcianu - górna ściana\n");
	printf("9. czytaj określone punkty\n");
	printf("10. podaj 8 wartości z zakresu 0-255\n");
	printf("11. zapisz wartości w podanych punktach\n");
	printf("0. koniec\n");
	scanf("%d",&choice);
	switch (choice) 
	{
	    case 1:
		printf("x1=");
		scanf(" %d",&x1);
		printf("y1=");
		scanf(" %d",&y1);
		printf("z1=");
		scanf(" %d",&z1);
		if ( x1<0 || x1>cube || y1<0 || y1>cube || z1<0 || z1>cube )
		{
    		    printf ("P1: poza zakresem!\n");
		    x1=-1;
		    y1=-1;
		    z1=-1;
		    break;
		}
		break;
	    case 2:
		printf("x2=");
		scanf(" %d",&x2);
		printf("y2=");
		scanf(" %d",&y2);
		printf("z2=");
		scanf(" %d",&z2);
		if ( x2<0 || x2>cube || y2<0 || y2>cube || z2<0 || z2>cube )
		{
    		    printf ("P2: poza zakresem!\n");
		    x2=-1;
		    y2=-1;
		    z2=-1;
		    break;
		}
		break;
	    case 3:
		printf("x3=");
		scanf(" %d",&x3);
		printf("y3=");
		scanf(" %d",&y3);
		printf("z3=");
		scanf(" %d",&z3);
		if ( x3<0 || x3>cube || y3<0 || y3>cube || z3<0 || z3>cube )
		{
    		    printf ("P3: poza zakresem!\n");
		    x3=-1;
		    y3=-1;
		    z3=-1;
		    break;
		}
		break;
	    case 4:
		printf("x4=");
		scanf(" %d",&x4);
		printf("y4=");
		scanf(" %d",&y4);
		printf("z4=");
		scanf(" %d",&z4);
		if ( x4<0 || x4>cube || y4<0 || y4>cube || z4<0 || z4>cube )
		{
    		    printf ("P4: poza zakresem!\n");
		    x4=-1;
		    y4=-1;
		    z4=-1;
		    break;
		}
		break;

	    case 5:
		printf("x5=");
		scanf(" %d",&x5);
		printf("y5=");
		scanf(" %d",&y5);
		printf("z5=");
		scanf(" %d",&z5);
		if ( x5<0 || x5>cube || y5<0 || y5>cube || z5<0 || z5>cube )
		{
    		    printf ("P5: poza zakresem!\n");
		    x5=-1;
		    y5=-1;
		    z5=-1;
		    break;
		}
		break;

	    case 6:
		printf("x6=");
		scanf(" %d",&x6);
		printf("y6=");
		scanf(" %d",&y6);
		printf("z6=");
		scanf(" %d",&z6);
		if ( x6<0 || x6>cube || y6<0 || y6>cube || z6<0 || z4>cube )
		{
    		    printf ("P6: poza zakresem!\n");
		    x6=-1;
		    y6=-1;
		    z6=-1;
		    break;
		}
		break;

	    case 7:
		printf("x7=");
		scanf(" %d",&x7);
		printf("y7=");
		scanf(" %d",&y7);
		printf("z7=");
		scanf(" %d",&z7);
		if ( x7<0 || x7>cube || y7<0 || y7>cube || z7<0 || z7>cube )
		{
    		    printf ("P7: poza zakresem!\n");
		    x7=-1;
		    y7=-1;
		    z7=-1;
		    break;
		}
		break;

	    case 8:
		printf("x8=");
		scanf(" %d",&x8);
		printf("y8=");
		scanf(" %d",&y8);
		printf("z8=");
		scanf(" %d",&z8);
		if ( x8<0 || x8>cube || y8<0 || y8>cube || z8<0 || z8>cube )
		{
    		    printf ("P8: poza zakresem!\n");
		    x8=-1;
		    y8=-1;
		    z8=-1;
		    break;
		}
		break;
	    case 9:
		if ( p1 ) 
		{
		    if ( z1<=za2 )
			printf ("<c1> P1(%d,%d,%d)=%d\n",x1,y1,z1,c1[x1*y1*z1]); //c1
		    if ( z1>=za3 && z1<=za4 )
			printf ("<c2> P1(%d,%d,%d)=%d\n",x1,y1,z1,c2[x1*y1*(z1-za3)]); //c2 -->z-620
		    if ( z1>=za5 && z1<=za6 )
			printf ("<c3> P1(%d,%d,%d)=%d\n",x1,y1,z1,c3[x1*y1*(z1-za5)]); //c3 -->z-1240
		}
		if ( p2 )
		{
		    if ( z2<=za2 )
			printf ("<c1> P2(%d,%d,%d)=%d\n",x2,y2,z2,c1[x2*y2*z2]); //c1
		    if ( z2>=za3 && z2<=za4 )
			printf ("<c2> P2(%d,%d,%d)=%d\n",x2,y2,z2,c2[x2*y2*(z2-za3)]); //c2 -->z-620
		    if ( z2>=za5 && z2<=za6 )
			printf ("<c3> P2(%d,%d,%d)=%d\n",x2,y2,z2,c3[x2*y2*(z2-za5)]); //c3 -->z-1240
		}
		if ( p3 )
		{
		    if ( z3<=za2 )
			printf ("<c1> P3(%d,%d,%d)=%d\n",x3,y3,z3,c1[x3*y3*z3]); //c1
		    if ( z3>=za3 && z3<=za4 )
			printf ("<c2> P3(%d,%d,%d)=%d\n",x3,y3,z3,c2[x3*y3*(z3-za3)]); //c2 -->z-620
		    if ( z3>=za5 && z3<=za6 )
			printf ("<c3> P3(%d,%d,%d)=%d\n",x3,y3,z3,c3[x3*y3*(z3-za5)]); //c3 -->z-1240
		}
		if ( p4 )
		{
		    if ( z4<=za2 )
			printf ("<c1> P4(%d,%d,%d)=%d\n",x4,y4,z4,c1[x4*y4*z4]); //c1
		    if ( z4>=za3 && z4<=za4 )
			printf ("<c2> P4(%d,%d,%d)=%d\n",x4,y4,z4,c2[x4*y4*(z4-za3)]); //c2 -->z-620
		    if ( z4>=za5 && z4<=za6 )
			printf ("<c3> P4(%d,%d,%d)=%d\n",x4,y4,z4,c3[x4*y4*(z4-za5)]); //c3 -->z-1240
		}
		if ( p5 )
		{
		    if ( z5<=za2 )
			printf ("<c1> P5(%d,%d,%d)=%d\n",x5,y5,z5,c1[x5*y5*z5]); //c1
		    if ( z5>=za3 && z5<=za4 )
			printf ("<c2> P5(%d,%d,%d)=%d\n",x5,y5,z5,c2[x5*y5*(z5-za3)]); //c2 -->z-620
		    if ( z5>=za5 && z5<=za6 )
			printf ("<c3> P5(%d,%d,%d)=%d\n",x5,y5,z5,c3[x5*y5*(z5-za5)]); //c3 -->z-1240
		}
		if ( p6 )
		{
		    if ( z6<=za2 )
			printf ("<c1> P6(%d,%d,%d)=%d\n",x6,y6,z6,c1[x6*y6*z6]); //c1
		    if ( z6>=za3 && z6<=za4 )
			printf ("<c2> P6(%d,%d,%d)=%d\n",x6,y6,z6,c2[x6*y6*(z6-za3)]); //c2 -->z-620
		    if ( z6>=za5 && z6<=za6 )
			printf ("<c3> P6(%d,%d,%d)=%d\n",x6,y6,z6,c3[x6*y6*(z6-za5)]); //c3 -->z-1240
		}
		if ( p7 )
		{
		    if ( z7<=za2 )
			printf ("<c1> P7(%d,%d,%d)=%d\n",x7,y7,z7,c1[x7*y7*z7]); //c1
		    if ( z7>=za3 && z7<=za4 )
			printf ("<c2> P7(%d,%d,%d)=%d\n",x7,y7,z7,c2[x7*y7*(z7-620)]); //c2 -->z-620
		    if ( z7>=za5 && z7<=za6 )
			printf ("<c3> P7(%d,%d,%d)=%d\n",x7,y7,z7,c3[x7*y7*(z7-1240)]); //c3 -->z-1240
		}
		if ( p8 )
		{
		    if ( z8<=za2 )
			printf ("<c1> P8(%d,%d,%d)=%d\n",x8,y8,z8,c1[x8*y8*z8]); //c1
		    if ( z8>=za3 && z8<=za4 )
			printf ("<c2> P8(%d,%d,%d)=%d\n",x8,y8,z8,c2[x8*y8*(z8-za3)]); //c2 -->z-620
		    if ( z8>=za5 && z8<=za6 )
			printf ("<c3> P8(%d,%d,%d)=%d\n",x8,y8,z8,c3[x8*y8*(z8-za5)]); //c3 -->z-1240
		}
		printf("hit enter\n");
		scanf("%s",&i);
		break;
	    case 10:
		printf("podaj 8 wartości z przedziału <0,255>\n");
		while (1)
		{
		    printf("\nwartość val1 dla P1:\n");
		    scanf(" %d", &val1);
		    printf("\nwartość val2 dla P2:\n");
		    scanf(" %d", &val2);
		    printf("\nwartość val3 dla P3:\n");
		    scanf(" %d", &val3);
		    printf("\nwartość val4 dla P4:\n");
		    scanf(" %d", &val4);
		    printf("\nwartość val5 dla P5:\n");
		    scanf(" %d", &val5);
		    printf("\nwartość val6 dla P6:\n");
		    scanf(" %d", &val6);
		    printf("\nwartość val7 dla P7:\n");
		    scanf(" %d", &val7);
		    printf("\nwartość val8 dla P8:\n");
		    scanf(" %d", &val8);

		    if 
		    ( 
			0<=val1 && val1<=255 && 0<=val2 && val2<=255 && 0<=val3 && val3<=255 && 0<=val4 && val4<=255 &&
			0<=val5 && val5<=255 && 0<=val6 && val6<=255 && 0<=val7 && val7<=255 && 0<=val8 && val8<=255 
		    )
		    {
			printf("podane wartości: val1=%d val2=%d val3=%d val4=%d \n",val1,val2,val3,val4);
			printf("podane wartości: val5=%d val6=%d val7=%d val8=%d \n",val5,val6,val7,val8);
//			return 0;
			break;
		    }
		    else
		    {
			val1=-1; // wartości do zapisania w p1..p4
			val2=-1;
			val3=-1;
			val4=-1;
			val5=-1; // wartości do zapisania w p1..p4
			val6=-1;
			val7=-1;
			val8=-1;
		    }
		}
		
		break;
	    case 11:


		if ( p1 ) 
		{
		    if ( z1<=za2 )
			c1[x1*y1*z1]=val1; //c1
		    if ( z1>=za3 && z1<=za4 )
			c2[x1*y1*(z1-za3)]=val1; //c2 -->z-620
		    if ( z1>=za5 && z1<=za6 )
			c3[x1*y1*(z1-za5)]=val1; //c3 -->z-1240
		}
		if ( p2 )
		{
		    if ( z2<=za2 )
			c1[x2*y2*z2]=val2; //c1
		    if ( z2>=za3 && z2<=za4 )
			c2[x2*y2*(z2-za3)]=val2; //c2 -->z-620
		    if ( z2>=za5 && z2<=za6 )
			c3[x2*y2*(z2-za5)]=val2; //c3 -->z-1240
		}
		if ( p3 )
		{
		    if ( z3<=za2 )
			c1[x3*y3*z3]=val3; //c1
		    if ( z3>=za3 && z3<=za4 )
			c2[x3*y3*(z3-za3)]=val3; //c2 -->z-620
		    if ( z3>=za5 && z3<=za6 )
			c3[x3*y3*(z3-za5)]=val3; //c3 -->z-1240
		}
		if ( p4 )
		{
		    if ( z4<=za2 )
			c1[x4*y4*z4]=val4; //c1
		    if ( z4>=za3 && z4<=za4 )
			c2[x4*y4*(z4-za3)]=val4; //c2 -->z-620
		    if ( z4>=za5 && z4<=za6 )
			c3[x4*y4*(z4-za5)]=val4; //c3 -->z-1240
		}
		if ( p5 )
		{
		    if ( z5<=za2 )
			c1[x5*y5*z5]=val5; //c1
		    if ( z5>=za3 && z5<=za4 )
			c2[x5*y5*(z5-za3)]=val5; //c2 -->z-620
		    if ( z5>=za5 && z5<=za6 )
			c3[x5*y5*(z5-za5)]=val5; //c3 -->z-1240
		}
		if ( p6 )
		{
		    if ( z6<=za2 )
			c1[x6*y6*z6]=val6; //c1
		    if ( z6>=za3 && z6<=za4 )
			c2[x6*y6*(z6-620)]=val6; //c2 -->z-620
		    if ( z6>=za5 && z6<=za6 )
			c3[x6*y6*(z6-1240)]=val6; //c3 -->z-1240
		}
		if ( p7 )
		{
		    if ( z7<=za2 )
			c1[x7*y7*z7]=val7; //c1
		    if ( z7>=za3 && z7<=za4 )
			c2[x7*y7*(z7-za3)]=val7; //c2 -->z-620
		    if ( z7>=za5 && z7<=za6 )
			c3[x7*y7*(z7-za5)]=val7; //c3 -->z-1240
		}
		if ( p8 )
		{
		    if ( z8<=za2 )
			c1[x8*y8*z8]=val8; //c1
		    if ( z8>=za3 && z8<=za4 )
			c2[x8*y8*(z8-za3)]=val8; //c2 -->z-620
		    if ( z8>=za5 && z8<=za6 )
			c3[x8*y8*(z8-za5)]=val8; //c3 -->z-1240
		}
		printf("hit enter\n");
		scanf("%s",&i);
		break;
	    case 0:
		free(c1);
		free(c2);
		free(c3);
		printf("mem cleared\n");
		return 0;
	    default:
		continue;

        }





//wyliczone położenie


}

printf("ctrl+c to quit\n");
while (1)
    {

    }

return 0;

}

