#include<stdio.h>
typedef enum
{
	jan,
	feb,
	march,
	april,
	may,
	june,
	july,
	aug,
	sep,
	oct,
	nov,
	dec,
}month;
int main()
{
	month season=jan;
	printf("it is %d\n",jan);
	switch(season)
	{
		case dec:
		case jan:
		case feb:
		case march:
		case april:
		case may:
			printf("winter\n");
			break;
		case june:
		case july:
		case aug:
		case sep:
			printf("summer\n");
			break;
		case oct:
		case nov:
			printf("rainy\n");
			break;
		default:printf("otherwise\n");
			break;

	}
}

