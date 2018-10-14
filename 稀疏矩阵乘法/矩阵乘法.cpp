/*
ϡ�����˷�
�������˷�(M(m1*n1)��N(m2,n2)=P(m1*n2))ʱ�临�Ӷ�ΪO(m1*n1*n2)
ϡ�����Ϊ����ľ��󣬲�����Ԫ�����Ԫ��
�������г������������ʱ�临�Ӷȿ�ΪO��m1*n2��
*/

#include <stdio.h>
#include <stdlib.h>
//������������������Ҿ���������
#define LEFT_MATRIX_ROW 3
#define LEFT_MATRIX_COL 4
#define RIGHT_MATRIX_ROW 4
#define RIGHT_MATRIX_COL 2

//����ṹ:ϡ�������Ԫ���洢��ʽ
typedef struct data3yuanElement{
	int dataCol;
	int dataRow;
	int dataValue;
};

//��������
//������ &�� ��֤�ں����з����ڴ���Դ���������
//����myMatrix�����ڴ��,����ʱ�ڴ汻�ͷ�
void inputMatrix(int** &myMatrix,int row,int col,int &numOfNotZero){
	//��̬�����ά����
	myMatrix=(int **)malloc(row*sizeof(int *));  
	for(int i=0;i<row;i++)  
		myMatrix[i]=(int *)malloc(col*sizeof(int)); 
	//�������
	printf("����%d*%d����",row,col);
	for (int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			scanf("%d",&myMatrix[i][j]);
			//ͳ�Ʒ�0Ԫ�ظ���,���ں���������Ԫ���ռ�
			if (myMatrix[i][j]!=0)
				numOfNotZero++;
		}
	}
}

//��ϡ�����ת��Ϊ��Ԫ���
void matrix2array(int** &myMatrix,struct data3yuanElement *data,
	int row,int col){
	int k=0;
	for (int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			if (myMatrix[i][j]!=0)
			{
				data[k].dataRow=i;
				data[k].dataCol=j;
				data[k].dataValue=myMatrix[i][j];
				k++;
			}
		}
	}
}

//�ҵ�ÿ��(��)��һ������Ԫ��������Ԫ�����λ�����
//ȷ������Ԫ��"��ַ",���ں���ֱ�Ӷ�λ����ѯԪ��λ��
//������: ��Ԫ���,�����,��λ���鳤��,��or��,���ض�λ����,ÿ��(��)����Ԫ��ͳ������
void findFirstNotZero(struct data3yuanElement *data,int dataLen,
	int numOfDim,int RowOrCol,int* &returnArray,int* &num){
		//ÿ��(��)����Ԫ��ͳ������
		num=(int*)malloc(sizeof(int)*numOfDim);
		//������
		for(int i=0;i<numOfDim;i++)
			num[i]=0;
		//�ж����У�ͳ�Ƹ���
		for (int i=0;i<dataLen;i++)
		{
			if (RowOrCol==1){
				int row=data[i].dataRow;
				num[row]++;	
			}else if(RowOrCol==2){
				int col=data[i].dataCol;
				num[col]++;	//�������������±�
			}
		}
		//���i��(��)�׸�����Ԫ������Ԫ�����λ��
		returnArray=(int*)malloc(sizeof(int)*numOfDim);
		returnArray[0]=0;
		for (int i=1;i<numOfDim;i++){
			returnArray[i]=returnArray[i-1]+num[i-1];
		}
}

void main(){
	int **leftMatrix=NULL;
	int **rightMatrix=NULL;
	int numOfLeftMatNotZero=0;
	int numOfRightMatNotZero=0;
	//input
	inputMatrix(leftMatrix,LEFT_MATRIX_ROW,LEFT_MATRIX_COL,numOfLeftMatNotZero);
	inputMatrix(rightMatrix,RIGHT_MATRIX_ROW,RIGHT_MATRIX_COL,numOfRightMatNotZero);
		
	struct data3yuanElement *dataLeft =(struct data3yuanElement*)
		malloc(sizeof(struct data3yuanElement)*numOfLeftMatNotZero);
	struct data3yuanElement *dataRight=(struct data3yuanElement*)
		malloc(sizeof(struct data3yuanElement)*numOfRightMatNotZero);
	//matrix to ��Ԫ��� 
	matrix2array(leftMatrix,dataLeft,LEFT_MATRIX_ROW,LEFT_MATRIX_COL);
	matrix2array(rightMatrix,dataRight,RIGHT_MATRIX_ROW,RIGHT_MATRIX_COL);
	
	int dataLeftLen=numOfLeftMatNotZero;
	int dataRightLen=numOfRightMatNotZero;

	int *leftColFirstNotZero=NULL;
	int *rightRowFirstNotZero=NULL;

	int *leftNumOfNotZeroEachCol=NULL;
	int *rightNumOfNotZeroEachRow=NULL;
	//��λ����
	findFirstNotZero(dataLeft,dataLeftLen,LEFT_MATRIX_COL,2,
		leftColFirstNotZero,leftNumOfNotZeroEachCol);
	findFirstNotZero(dataRight,dataRightLen,RIGHT_MATRIX_ROW,1,
		rightRowFirstNotZero,rightNumOfNotZeroEachRow);
		
	//�������
	int matrixNew[LEFT_MATRIX_ROW][RIGHT_MATRIX_COL];
	for (int i=0;i<LEFT_MATRIX_ROW;i++)
	{
		for (int j=0;j<RIGHT_MATRIX_COL;j++)
		{
			matrixNew[i][j]=0;
		}
	}

	//�������Ԫ������
	int leftRow=0, leftCol=0;
	int rightRow=0,rightCol=0;
	//����(�Ҿ���)���Ԫ��������Ԫ�����λ�����
	int inRightNum=0;	
	//����������Ӧ��Ԫ���
	//����Ԫ�ؿ������,������==����
	for (int i=0;i<dataLeftLen;i++)
	{
		leftRow=leftCol=rightRow=inRightNum=rightCol=0;
		leftCol=rightRow=dataLeft[i].dataCol;	//�õ���������Ԫ�ص��кţ��Ҿ����кţ�
		leftRow=dataLeft[i].dataRow;			//������к�
							
		for (int j=0;j<rightNumOfNotZeroEachRow[rightRow];j++)
		{	//���Ҿ����Ӧ��Ԫ�������
			//�����ĳһԪ�أ���Ϊx���������Ҿ�����Ϊx����Ԫ����ˣ�������ѭ��
			//ȥ����ЩԪ������Ԫ������λ�� ��Ҫ����ַ+��ַ
			//��ַΪ�׸����з���Ԫ�أ���ַΪѭ������j������ʽ
			inRightNum=rightRowFirstNotZero[rightRow]+j;										
			rightCol=dataRight[inRightNum].dataCol;		//�Ҿ����к�
			matrixNew[leftRow][rightCol]+=
				dataLeft[i].dataValue * dataRight[inRightNum].dataValue;   //��˼���ԭֵ
		}		
	}

	//��ʾ���
	for (int i=0;i<LEFT_MATRIX_ROW;i++)
	{
		for(int j=0;j<RIGHT_MATRIX_COL;j++){
			printf("%d ",matrixNew[i][j]);
		}
		printf("\n");
	}

}