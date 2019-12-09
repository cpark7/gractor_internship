
# coding: utf-8

# In[1]:


import paho.mqtt.client as mqtt
import ssl
import time
import serial
import signal
import threading
line = [] #라인 단위로 데이터 가져올 리스트 변수


host = "nas.gractor.com"
port = 1883
topic = "/data/pms/mj"  # your mqtt topic
username = "gractor"  # your mqtt username
password = "gractor1234"  # your generated api key
serial_port = '/dev/ttyUSB0' # 시리얼 포트
baud = 115200 # 시리얼 보드레이트(통신속도)
exitThread = False   # 쓰레드 종료용 변수
msg = ""


def on_connect(client, userdata, flags, rc):
    print(mqtt.connack_string(rc))

# Callback triggered by a new Pozyx data packet
def on_message(client, userdata, msg):
    print("Positioning update:", msg.payload.decode())

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed to topic!")

client2 = mqtt.Client()
client2.username_pw_set(username, password=password)

# sets the secure context, enabling the WSS protocol
#client2.tls_set_context(context=ssl.create_default_context())

# set callbacks
client2.on_connect = on_connect
client2.on_message = on_message
client2.on_subscribe = on_subscribe
client2.connect(host, port=port)


#쓰레드 종료용 시그널 함수
def handler(signum, frame):
     exitThread = True


#데이터 처리할 함수
def parsing_data(data):
    # 리스트 구조로 들어 왔기 때문에
    # 작업하기 편하게 스트링으로 합침
    tmp = ''.join(data)

    #출력!
    print(tmp)

#본 쓰레드
def readThread(ser):
    global line
    global exitThread
    global msg
    # 쓰레드 종료될때까지 계속 돌림
    while not exitThread:
        #데이터가 있있다면
        for c in ser.read():
            #line 변수에 차곡차곡 추가하여 넣는다.
            line.append(chr(c))
            msg +=(str(chr(c)))
            if c == 10: #라인의 끝을 만나면..
                #데이터 처리 함수로 호출
                parsing_data(line)
                client2.publish(topic,msg)
                #line 변수 초기화
                del line[:] 
                msg = ""

if __name__ == "__main__":
    #종료 시그널 등록
    signal.signal(signal.SIGINT, handler)

    #시리얼 열기
    ser = serial.Serial(serial_port, baud, timeout=0)

    #시리얼 읽을 쓰레드 생성
    thread = threading.Thread(target=readThread, args=(ser,))

    #시작!
    thread.start()
    


# In[ ]:





# In[1]:




