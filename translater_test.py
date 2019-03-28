import os
import sys
import urllib.request #표준 라이브러리
import json

client_id = "vIj2xvkAVXo8ArNz48NH" # 개발자센터에서 발급받은 Client ID 값
client_secret = "baRtDmvE49" # 개발자센터에서 발급받은 Client Secret 값
# 요청할 주소
url = "https://openapi.naver.com/v1/papago/n2mt"

def API():
    #번역할 언어와 내용
    text = input("번역할 내용을 입력하세요: ")
    encText = urllib.parse.quote(text)
    srcLang = 'en'
    tarLang = 'ko'
    data = "source={}&target={}&text=".format(srcLang, tarLang) + encText

    #웹 요청
    request = urllib.request.Request(url)
    request.add_header("X-Naver-Client-Id", client_id)
    request.add_header("X-Naver-Client-Secret", client_secret)

    #결과 받아오는 부분
    response = urllib.request.urlopen(request, data=data.encode("utf-8"))

    #응답 성공적일 때
    rescode = response.getcode()
    if(rescode == 200): #성공
        response_body = response.read()
        data = response_body.decode('utf-8')
        data = json.loads(data) #딕셔너리화
        trans_text = data['message']['result']['translatedText']
        print("번역된 내용: ", trans_text)
    else: #실패
        print("Error Code:" + rescode)

API()