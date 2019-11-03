# OJJJ
개인 공부 공간


<ctrl +f>로 <branch name>을 찾아서 확인하세요

-----------------------------------------------------------------HI-----------------------------------------------------------------

--------------------------------------------------------------SERVER--------------------------------------------------------------

목적 - 캠으로 촬영된 영상을 서버로 전송했다가 다시 수신해서 출력하기
       서버는 AWS ec2 ubuntu.
       
이전의 내용 - <tcp_Server ver1.0.c> / <Client ver Python 1.0.py> / Client ver C++ 1.0.cpp> / <Header to Client C++ 1.0.h>

190703 - 캠으로 촬영된 영상의 순간 프레임을 서버로 전송하기 위해서는 데이터의 변환이 필요. 
         Mat(프레임)을 uchar*(변환된 형태)로 변환은 기본 제공해줌.
         서버로 전송및 수신하기 위해선 char형 배열 또는 포인터로 다루어야 할 것 같음.
       
       1) uchar* -> char* 변화시 데이터 손실 없음을 확인. 
       2) char*형태로 서버 데이터 송신 가능 확인.
       3) Mat -> uchar* -> char*(송신) -> char*(수신) -> uchar* -> Mat -> 출력.
       4) 문제 없이 작동함을 확인. 딜레이는 약 0.16 ~ 0.17 사이
       < udp_Server ver 1.0.c> / < Client ver C++ 1.1.cpp >
       +++ 다중 클라이언트에게 멀티캐스트? 브로드캐스트? 하기, 윈도우 프로그램으로 UI만들기 
       
       
190715 - MFC에서 촬영된 캠을 서버를 통해 에코잉 받을 수 있어야 함.
         멀티캐스트 방식은 힘들 것 같음. 그냥 멀티 플렉싱을 활용해야 할 것 같음.

       1) MFC에서 기본적으로 제공하는 라이브러리로 시도.
       2) 데이터 송수신엔 문제가 없음을 확인
       3) 데이터 송신할때의 Mat 변수의 크기와 동일한 크기로 수신해야 에러가 없음.
       4) 수신한 영상을 MFC의 특정 Picture control 에 출력하는데에 무리가 있음.
       < Client ver MFC 1.0.cpp > / < Header to MFC 1.0.h >
       +++ 두 클라이언트간의 영상 송수신 테스트하기.
       
190716 - 서버 통신 방식을 재정립할 필요가 있다.

       1) 멀티캐스팅은 EC2에서 지원하지 않으므로 불가
       2) 멀티플렉싱과 더불어 멀티 쓰레드는 영상 송수신에 있어서 TCP 방식이 어려움
       3) UDP 방식으로 하려니 클라이언트에게 전송할 각 데이터를 구분짓기 어려움
          ( A 클라이언트 입장에서 송신된 데이터가 B의 데이터인지 C의 데이터인지 모름 )
          
190830 - 서버 통신 방식을 epoll로 결정, 멀티 쓰레드 구현 필요
       
       1) I/O 멀티 플랙싱을 통해서 서버와 클라이언트간의 제어 명령을 보내기로 결정
       2) select 방법을 이용시 소켓 번호가 많아질시 연산이 많아질 것 같음
       3) epoll 방법으로 멀티 플렉싱 구현성공.
       4) 쓰레드를 통해 추가 연결을 받으려니 기존의 epoll 연결과 겹침 발생
       < Server tcp Epoll 1.0v .c >
       +++ 연결된 epoll소켓으로 각 클라이언트에게 제어명령을 보내고, 멀티쓰레드로 해당 제어를 통제 해야함.
            멀티 쓰레드로 영상을 받고, 전송하기 위해서, 각 클라이언트별로 쓰레드와 소켓을 생성하는 방법을 찾아야함.
            연결된 클라이언트 주소 대조를 통해서 적절하게 연결해야할 필요가 있을것 같음.
               
190903 - 멀티 쓰레드 구현. 동기화 및 영상 전송 여부 확인 필요

       1) epoll 기능을 기본으로 멀티 쓰레드 구현.
       2) 클라이언트에서 서버로 연결할때 제어신호를 통해 연결된 목적을 판단하여 쓰레드로 돌리는 방식.
       3) 클라이언트 별로 각각의 데이터를 전송하고, 클라이언트별로 전송된 데이터 다시 각 클라이언트에 보내는 방식.
       4) 현재 단순한 정수 값만으로 테스트, 영상으로 전송시켜봐야함.
       5) 여러 클라이언트에서 동시다발적으로 연결요청이 발생할때, 연결에 있어서 에러 발생.
       6) 또한 요청 - 수락 과정에서 데이터의 송수신이 지연될 시 누락된 제어신호와 함께 에러 발생.
       7) MFC로 구현 안함.
       < Server tcp Epoll 1.1v .c, Server tcp Epoll 1.2v .c, Client tcp Epoll 1.2v .c -test >
       +++ 영상데이터를 주고받을경우 정상적으로 작동하는지 확인해야함.
            동시다발적으로 연결이 요청되었을때, 발생하는 에러를 해결하던지, 동기화 하는 방법을 찾던지 해야함.
            MFC로 구현해야함.
            
190923 - 다시 UDP로, 멀티 포팅(?) 으로 변경

       1) tcp 기반의 멀티 쓰레딩으로 시도했으나 연상전송에 무리가 있다고 판단, 영상전송에 용이한 UDP로 변경.
       2) 기존의 방식대로 기본연결엔 TCP기반의 epoll방식을 사용 영상을 송수신하는 각 쓰레드에선 UDP방식.
       3) 각 클라이언트당 포트번호를 배정하고, 특정 포트로의 데이터 전송으로 클라이언트간의 연결 구분.
       4) 영상 전송 데이터와 클라이언트 연결 제어 데이터간의 사이즈를 통해 데이터 구분
       +++ 아직 테스트는 못해봄 두개이상의 클라이언트로 테스팅필요. 

191102 - 처음으로.. 영상 전송부터.../ 이전에 영상 전송이 성공했다고 생각한건
         영상전송이 아닌 그저 버퍼에 남아있던 데이터를 다시 읽었기 때문이였음. 따라서 다시 영상을 전송하는 법을 찾아보아야함.
         
       1) Mat 데이터가 기본적으로 uchar 데이터로는 지원을 해주기 때문에 uchar 데이터 전송하는 법을 찾으면 될것 같다.
       2) uchar는 지원해 주는데 문제는, uchar가 가르키는 데이터의 사이즈나, uchar가 몇차원 포인터인지 모른다. 그걸 찾자.
       3) Mat 이 제공하는 uchar는 포인터 형식. uchar를 char의 포인터로 변환은 쉬우며, 포인터 자체로 socket에 담아서 전송은 가능.
       4) 다만 포인터로 socket에 담아 전송을 하려면 그에 맞는 원본 데이터의 size or lenth를 알아야 함.
       5) Mat이 제공하는 uchar 데이터의 크기를 알아야 전송이 가능한데, 사이즈를 찾는것이 우선이며, 일단 추측은 3차원 배열이다.
       
191103 - Mat data -> uchar* -> char* 는 틀린것 같음. Mat -> byte -> 가 맞는것 같음

       1) Mat -> byte -> Mat 다시 출력하는 것 까지는 성공. 이때 발생하는 delay가 생각보다 크지 않음.
       2) byte 를 전송하는 방법을 찾아야함.
       3) byte를 단순히 (char*)로 형변환해서 전송하면 되기는 함. 근데 이미지의 크기 때문에 한번에 전송이 안됨.
