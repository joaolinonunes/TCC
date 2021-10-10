#include <opencv2/imgcodecs.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


 Mat imgGray, imgBlur, imgCanny, imgDil, imgMedian; //declara o nome das imagens
 
 void getContours(Mat imgDil, Mat img){
	 vector<vector<Point>> contours;// declara um vetor com todos os contornos de entrada
	 vector<Vec4i> hierarchy;// declaara um vetor, contendo informações sobre a topologia da imagem. Possui tantos elementos quanto o número de contornos.
	 
	 findContours(imgDil,contours, hierarchy, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
	 //CHAIN_APPROX_SIMPLE, todos os pontos de fronteira são armazenados, Ele remove todos os pontos redundantes e comprime o contorno, economizando memória.
	 
	 drawContours(img,contours, -1, Scalar(0,255,255),2);
	 //Para desenhar os contornos, a função cv.drawContours é usada.
	 
	 for (int i = 0; i < contours.size(); i++)
	 {
		 int area = contourArea(contours[i]);
		 cout << area << endl;
		 
		 }
	 
	 
	 }

int main(){
	
	string path = "/home/joao/Área de Trabalho/contorno/defeito2.jpg"; //o caminho onde esta a imagem
	Mat img = imread(path); //classe Mat armazena imagem matriz
	
	//cout << img.size() <<endl; //mostra o tamanho da imagem
	
	cvtColor(img,imgGray, COLOR_BGR2GRAY); //deixa a imagem cinza
	GaussianBlur(imgGray, imgBlur, Size(3, 3),3,0); //borra a imagem quanto maior o numero mais borra
	Canny(imgBlur, imgCanny, 11, 85); //identifica bordas nas imagens, usa uma imagem borrada
	medianBlur(imgGray, imgMedian, 9);// desfoque medio altamente eficaz contra o ruido sal e pimenta, quanto maior o valor maior o desfoque os valores so podem ser impar e assima de 1
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//cria um nucleo, define a espessura das linhas de contorno maior os numeros maior aespessura da linha. numeros pecisão ser iguais.
	dilate(imgCanny, imgDil, kernel); //dilata as linhas da borda.
	
	getContours(imgDil,img);
	
	imshow("Imagem",img); //mostra a imagem 
	imshow("Imagem Median",imgMedian); //mostra a imagem 
	imshow("Imagem Gray", imgGray); //mostra a imagem com cinza 
	imshow("Imagem Blur", imgBlur); //mostra a imagem borrada
	imshow("Imagem Canny",imgCanny); //mostra somente as bordas da imagem
	imshow("Imagem Dilation", imgDil); //mostra somente as bordas da imagem dilatadas
	waitKey(0);
	
	return 0;
	}
