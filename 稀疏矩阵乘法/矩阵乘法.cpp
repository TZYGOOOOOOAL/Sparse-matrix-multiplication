/*
稀疏矩阵乘法
常规矩阵乘法(M(m1*n1)×N(m2,n2)=P(m1*n2))时间复杂度为O(m1*n1*n2)
稀疏矩阵为特殊的矩阵，采用三元组表存放元素
采用下列程序，理想情况下时间复杂度可为O（m1*n2）
*/

#include <stdio.h>
#include <stdlib.h>
//定义左矩阵行列数，右矩阵行列数
#define LEFT_MATRIX_ROW 3
#define LEFT_MATRIX_COL 4
#define RIGHT_MATRIX_ROW 4
#define RIGHT_MATRIX_COL 2

//定义结构:稀疏矩阵三元组表存储方式
typedef struct data3yuanElement{
	int dataCol;
	int dataRow;
	int dataValue;
};

//矩阵输入
//参数中 &符 保证在函数中分配内存可以传回主函数
//否则myMatrix分配内存后,返回时内存被释放
void inputMatrix(int** &myMatrix,int row,int col,int &numOfNotZero){
	//动态分配二维数组
	myMatrix=(int **)malloc(row*sizeof(int *));  
	for(int i=0;i<row;i++)  
		myMatrix[i]=(int *)malloc(col*sizeof(int)); 
	//输入矩阵
	printf("输入%d*%d矩阵：",row,col);
	for (int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			scanf("%d",&myMatrix[i][j]);
			//统计非0元素个数,便于后续分配三元组表空间
			if (myMatrix[i][j]!=0)
				numOfNotZero++;
		}
	}
}

//将稀疏矩阵转化为三元组表
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

//找到每行(列)第一个非零元素所在三元组表中位置序号
//确定非零元素"基址",便于后续直接定位带查询元素位置
//参数表: 三元组表,组表长度,定位数组长度,行or列,返回定位数组,每行(列)非零元素统计数组
void findFirstNotZero(struct data3yuanElement *data,int dataLen,
	int numOfDim,int RowOrCol,int* &returnArray,int* &num){
		//每行(列)非零元素统计数组
		num=(int*)malloc(sizeof(int)*numOfDim);
		//先置零
		for(int i=0;i<numOfDim;i++)
			num[i]=0;
		//判断行列，统计个数
		for (int i=0;i<dataLen;i++)
		{
			if (RowOrCol==1){
				int row=data[i].dataRow;
				num[row]++;	
			}else if(RowOrCol==2){
				int col=data[i].dataCol;
				num[col]++;	//巧妙利用数组下标
			}
		}
		//求第i行(列)首个非零元素在三元组表中位置
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
	//matrix to 三元组表 
	matrix2array(leftMatrix,dataLeft,LEFT_MATRIX_ROW,LEFT_MATRIX_COL);
	matrix2array(rightMatrix,dataRight,RIGHT_MATRIX_ROW,RIGHT_MATRIX_COL);
	
	int dataLeftLen=numOfLeftMatNotZero;
	int dataRightLen=numOfRightMatNotZero;

	int *leftColFirstNotZero=NULL;
	int *rightRowFirstNotZero=NULL;

	int *leftNumOfNotZeroEachCol=NULL;
	int *rightNumOfNotZeroEachRow=NULL;
	//求定位数组
	findFirstNotZero(dataLeft,dataLeftLen,LEFT_MATRIX_COL,2,
		leftColFirstNotZero,leftNumOfNotZeroEachCol);
	findFirstNotZero(dataRight,dataRightLen,RIGHT_MATRIX_ROW,1,
		rightRowFirstNotZero,rightNumOfNotZeroEachRow);
		
	//结果数组
	int matrixNew[LEFT_MATRIX_ROW][RIGHT_MATRIX_COL];
	for (int i=0;i<LEFT_MATRIX_ROW;i++)
	{
		for (int j=0;j<RIGHT_MATRIX_COL;j++)
		{
			matrixNew[i][j]=0;
		}
	}

	//定义相乘元素坐标
	int leftRow=0, leftCol=0;
	int rightRow=0,rightCol=0;
	//定义(右矩阵)相乘元素在右三元组表中位置序号
	int inRightNum=0;	
	//遍历左矩阵对应三元组表
	//若两元素可以相乘,则左列==右行
	for (int i=0;i<dataLeftLen;i++)
	{
		leftRow=leftCol=rightRow=inRightNum=rightCol=0;
		leftCol=rightRow=dataLeft[i].dataCol;	//得到左矩阵非零元素的列号（右矩阵行号）
		leftRow=dataLeft[i].dataRow;			//左矩阵行号
							
		for (int j=0;j<rightNumOfNotZeroEachRow[rightRow];j++)
		{	//在右矩阵对应三元组中序号
			//左矩阵某一元素（列为x）会与多个右矩阵（行为x）的元素相乘，所以有循环
			//去找这些元素在三元数组中位置 需要：基址+变址
			//基址为首个该行非零元素，变址为循环变量j，如下式
			inRightNum=rightRowFirstNotZero[rightRow]+j;										
			rightCol=dataRight[inRightNum].dataCol;		//右矩阵列号
			matrixNew[leftRow][rightCol]+=
				dataLeft[i].dataValue * dataRight[inRightNum].dataValue;   //相乘加上原值
		}		
	}

	//显示输出
	for (int i=0;i<LEFT_MATRIX_ROW;i++)
	{
		for(int j=0;j<RIGHT_MATRIX_COL;j++){
			printf("%d ",matrixNew[i][j]);
		}
		printf("\n");
	}

}