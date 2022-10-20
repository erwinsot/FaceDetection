#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h> 
#include <pthread.h>
#define THREADS 4

using namespace cv;

using namespace std;

void detectFace(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale,int fcc,Size frame_size);
string cascadeName, nestedCascadeName;
double scale=1;
int main(int argc, char** argv)
{
    clock_t t;
    t=clock();
	VideoCapture cap;
	Mat frame, image;

    ofstream ofs("Data.txt");
    
    
	CascadeClassifier cascade, nestedCascade;
	nestedCascade.load("/usr/share/opencv4/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
	cascade.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml");
	cap.open(argv[1]);
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT); 
    Size frame_size(frame_width, frame_height);
    int fcc=cv::VideoWriter::fourcc('X','V','I','D');
    double fps = cap.get(CAP_PROP_FPS);
    cout<< "los frames por segfundo son "<<fps<<endl;
    VideoWriter writer;
    writer = VideoWriter(argv[2] + std::string(".avi"),fcc,fps,frame_size,true);        
    
    int total_frames = cap.get(CAP_PROP_FRAME_COUNT);
    cout<< total_frames<< endl;
    int minutes = 0;
    int seconds = 28;
    int frame_id = int(fps*(minutes*60 + seconds));
    cout<<"frame id ="<<frame_id<<endl;
      cap.set(CAP_PROP_POS_FRAMES,105);
	  if (cap.isOpened())
            {
                        // Capturando cada frame del video
                        cout << "Face Detection is started...." << endl;
                        while (1)
                        {
                                    cap >> frame;
                                    double num_frame=cap.get(CAP_PROP_POS_FRAMES);
                                    //cout<< num_frame <<endl;
                                    //cout << cap << endl;
                                    if (frame.empty()){
                                        if (!ofs.bad())
                                            {
                                                t = clock()-t;
                                                double time = (double(t)/CLOCKS_PER_SEC);
                                                ofs << "El tiempo de ejcucion es" << endl;
                                                ofs << time << endl;
                                                ofs.close();
                                            }

                                                break;
                                    }
                                    stringstream file;
                                    file<<"/home/erwin/Escritorio/taller_para/frames/frame"<<num_frame<<".jpg";
                                    cv::imwrite(file.str(),frame);
                                    Mat frame1 = frame.clone();
                                    cv::resize(frame1, frame1, Size(1000, 700), 0, 0, INTER_CUBIC);
                                    vector<Rect> faces;
                                    Mat gray, smallImg;
                                    // convirtiendo cada frame a color gris
                                    cvtColor(frame, gray, COLOR_BGR2GRAY);
                                    double fx = 1 / scale;
                                    // cambiando el tamaño de los frames
                                    resize(gray, smallImg, Size(),fx, fx, INTER_LINEAR);
                                    equalizeHist(smallImg, smallImg);
                                    cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
                                    
                                    // dibujando cada frame
                                    for (size_t i = 0; i < faces.size(); i++)
                                    {

                                                Rect r = faces[i];
                                                Mat smallImgROI;
                                                vector<Rect> nestedObjects;                                                
                                                Scalar color = Scalar(255, 0, 0);                                                
                                                                
                                               
                                               /// dibujar cuadrado de cara
                                                            rectangle(frame, Point(cvRound(r.x * scale), cvRound(r.y * scale)),
                                                                        Point(cvRound((r.x + r.width - 1) * scale),
                                                                                    cvRound((r.y + r.height - 1) * scale)), color, 3, 8, 0);




                                                //algoritmo para difuminar el rostro                                                            
                                                cv::Point topLeft = cv::Point(cvRound(r.x * scale), cvRound(r.y * scale));
                                                cv::Point bottomRight = cv::Point(cvRound((r.x + r.width - 1) * scale),cvRound((r.y + r.height - 1) * scale));
                                                cv::Rect roi = cv::Rect(topLeft, bottomRight);                                 

                                                cv::GaussianBlur(frame(roi), frame(roi), cv::Size(91, 91), 0);
                                                            

                                                            
                                                if (nestedCascade.empty())
                                                            continue;
                                                smallImgROI = smallImg(r);
                                    }
                                    //
                                    writer.write(frame);                                     
                                    
                                    // Para mostar video 

                                    imshow("Face Detection", frame1);                                        
				
                                    
                                    char c = (char)waitKey(10);
                                    // Press q to exit from the window
                                    if (c == 27 || c == 'q' || c == 'Q')
                                                break;
                        }
            }
	else
                        cout << "Could not Open Video/Camera! ";
            return 0;

 
 





	

}

