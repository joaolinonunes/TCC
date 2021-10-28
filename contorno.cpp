#include <opencv2/imgcodecs.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
RNG rng(12345);

 Mat imgGray, imgBlur, imgCanny, imgDil, imgMedian; //declara o nome das imagens
 
 void getContours(Mat imgDil, Mat img){
	 vector<vector<Point>> contours;// declara um vetor com todos os contornos de entrada
	 vector<Vec4i> hierarchy;// declaara um vetor, contendo informações sobre a topologia da imagem. Possui tantos elementos quanto o número de contornos.
	 
	 /*findContours(imgDil,contours, hierarchy, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
	 //CHAIN_APPROX_SIMPLE, todos os pontos de fronteira são armazenados, Ele remove todos os pontos redundantes e comprime o contorno, economizando memória.
	 
	 drawContours(img,contours, -1, Scalar(127,255,255),2);
	 //Para desenhar os contornos, a função cv.drawContours é usada.
	 
	 for (int i = 0; i < contours.size(); i++)
	 {
		 
		 int area = contourArea(contours[i]);
		 cout << area << endl;
		 
		 
		 }*/
		 
		  vector<vector<Point> > contoursSOS;
		  //vector<Vec4i> hierarchy;

		  /// Detect edges using Threshold
		  //threshold( DOS_gray, threshold_outputSO, 0, 255, THRESH_BINARY );
		  /// Find contours
		  findContours( imgDil, contoursSOS, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
		  
		  /// Approximate contours to polygons + get bounding rects and circles
		  vector<vector<Point> > contours_poly( contoursSOS.size() );
		  vector<Rect> boundRect( contoursSOS.size() );
		  vector<Point2f>center( contoursSOS.size() );
		  vector<float>radius( contoursSOS.size() );

		  for( size_t i = 0; i < contoursSOS.size(); i++ )
			 { approxPolyDP( Mat(contoursSOS[i]), contours_poly[i], 3, true );
			   boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			   minEnclosingCircle( contours_poly[i], center[i], radius[i] );
			 }


		  /// Get the moments
		  vector<Moments> mu(contoursSOS.size() );
		  for( int i = 0; i < contoursSOS.size(); i++ ){ 
			  mu[i] = moments( contoursSOS[i], false ); 
			}

		  ///  Get the mass centers:
		  vector<Point2f> mc( contoursSOS.size() );
		  for( int i = 0; i < contoursSOS.size(); i++ ){
			  mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		  }
				 			   
			float disteuclidiana;

			/// Draw polygonal contour + bonding rects + circles
			Mat drawingA = Mat::zeros( imgDil.size(), CV_8UC1 );

		 for( int i = 0; i< contoursSOS.size(); i++ ){
				
				Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) ); 
			 
			 	disteuclidiana = sqrt(pow((boundRect[i].tl().y - boundRect[i].br().y),2) + pow((boundRect[i].tl().x - boundRect[i].br().x),2));
				if((disteuclidiana>60)&&(disteuclidiana<1200)){
					   //drawContours( drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
					   rectangle( drawingA, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
					   
					   
				}
		}
	 
	 imshow("Contour DOS", drawingA);
	 
	 }

int main(){
	
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));//cria um nucleo, define a espessura das linhas de contorno maior os numeros maior aespessura da linha. numeros pecisão ser iguais.
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(5, 5));
	string path = "/home/joao/Área de Trabalho/contorno/teste.jpg"; //o caminho onde esta a imagem
	Mat img = imread(path); //classe Mat armazena imagem matriz
	
	//cout << img.size() <<endl; //mostra o tamanho da imagem
	
	
	cvtColor(img,imgGray, COLOR_BGR2GRAY); //deixa a imagem cinza
	erode(imgGray, imgDil, kernel);
	GaussianBlur(imgDil, imgBlur, Size(3, 3),3,0); //borra a imagem quanto maior o numero mais borra
	Canny(imgBlur, imgCanny, 11, 85); //identifica bordas nas imagens, usa uma imagem borrada
	medianBlur(imgGray, imgMedian, 9);// desfoque medio altamente eficaz contra o ruido sal e pimenta, quanto maior o valor maior o desfoque os valores so podem ser impar e assima de 1
	
	//dilate(imgCanny, imgDil, kernel); //dilata as linhas da borda.
	//erode(imgCanny, imgDil, kernel2);
	//erode(imgGray, imgDil, kernel);
	getContours(imgCanny,img);
	
	imshow("Imagem",img); //mostra a imagem 
	imshow("Imagem Median",imgMedian); //mostra a imagem 
	imshow("Imagem Gray", imgGray); //mostra a imagem com cinza 
	imshow("Imagem Blur", imgBlur); //mostra a imagem borrada
	imshow("Imagem Canny",imgCanny); //mostra somente as bordas da imagem
	imshow("Imagem Erosão", imgDil); //mostra somente as bordas da imagem dilatadas
	waitKey(0);
	
	return 0;
	}
