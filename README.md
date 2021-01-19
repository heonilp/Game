# Game 및 관련 프로젝트

## 객체 지향 인베이더 콘솔 게임

<div>
<img src="https://github.com/heonilp/Game/blob/master/C%2B%2B%20%EA%B0%9D%EC%B2%B4%EC%A7%80%ED%96%A5%20%EC%9D%B8%EB%B2%A0%EC%9D%B4%EB%8D%94%20%EA%B2%8C%EC%9E%84%2C%20%ED%8C%8C%EC%84%9C%2C%20%EB%A6%AC%EC%8A%A4%ED%8A%B8/%EC%9D%B8%EB%B2%A0%EC%9D%B4%EB%8D%94%20%EA%B2%B0%EA%B3%BC%20%ED%99%94%EB%A9%B4.png" width="70%"></img>
</div>

- C++, 콘솔게임 (exe파일 다운 받으면 가능)
- 파서 이용
- [프로젝트 링크](https://github.com/heonilp/Game/tree/master/C%2B%2B%20%EA%B0%9D%EC%B2%B4%EC%A7%80%ED%96%A5%20%EC%9D%B8%EB%B2%A0%EC%9D%B4%EB%8D%94%20%EA%B2%8C%EC%9E%84%2C%20%ED%8C%8C%EC%84%9C%2C%20%EB%A6%AC%EC%8A%A4%ED%8A%B8)

- [유튜브 동영상](https://www.youtube.com/watch?v=60MMLaZS-MQ&feature=youtu.be)

### 구조
1. 파서(파일입출력 : invader.txt)를 이용한 인베이더 난이도 조정
2. 자료구조 이중 리스트을 이용한 데이터 (몬스터, 미사일, 플레이어 등 )관리 
3. 클래스를 이용한 상속관계
4. 2단계까지 난이도 만듬

<div>
<img src="https://github.com/heonilp/Game/blob/master/C%2B%2B%20%EA%B0%9D%EC%B2%B4%EC%A7%80%ED%96%A5%20%EC%9D%B8%EB%B2%A0%EC%9D%B4%EB%8D%94%20%EA%B2%8C%EC%9E%84%2C%20%ED%8C%8C%EC%84%9C%2C%20%EB%A6%AC%EC%8A%A4%ED%8A%B8/%EC%9D%B8%EB%B2%A0%EC%9D%B4%EB%8D%94%20%EC%8B%9C%EC%9E%91%ED%95%98%EA%B8%B0%20%EB%A9%94%EC%9D%B8%20%EA%B2%B0%EA%B3%BC%ED%99%94%EB%A9%B4.png" width="70%"></img>
</div>

```
게임 사용법
F1 : 게임 시작
스페이스 : 공격 , 방향 : 이동
(이 내용은 ReadMe.txt에도 있습니다.)
지면 다시 시작하는 것도 있습니다.
```

## JSON 을 이용한 친구 관리 시스템 및 채팅 욕 DB 거르기

<div>
<img src="https://github.com/heonilp/Game/blob/master/Friend%20%EA%B4%80%EB%A6%AC%2C%20%EB%8D%94%EB%AF%B8(%EC%84%9C%EB%B2%84)%2C%20JSON%EC%9D%B4%EC%9A%A9%2C%20%ED%8C%A8%ED%82%B7%EC%A7%81%EB%A0%AC%ED%99%94%ED%81%90/%EC%B9%9C%EA%B5%AC%20%EA%B4%80%EB%A6%AC%20%EA%B2%B0%EA%B3%BC%ED%99%94%EB%A9%B4.png" width="70%"></img>
</div>

- 언어: C++
- List, JSON이용, 패킷직렬화큐, 스트림 큐

1. JSON 을 이용한 친구 관리 시스템 
 - 1.1 JSON 이용 , 모델 :Select 
 - 1.2 자료구조 STL 리스트을 이용한 친구 데이터 관리

<div>
<img src="https://github.com/heonilp/Game/blob/master/%EC%B1%84%ED%8C%85%20%EC%9A%95%20DB%20%EA%B1%B0%EB%A5%B4%EA%B8%B0%20%EC%97%90%EC%BD%94%20%EC%84%9C%EB%B2%84%2C%ED%81%B4%EB%9D%BC%20IOCP%2C%20%EC%8A%A4%ED%8A%B8%EB%A6%BC%20%ED%81%90%20%ED%8F%AC%ED%95%A8/%EA%B2%B0%EA%B3%BC2.png" width="70%"></img>
</div>


2. 채팅 욕 DB 거르기
  - 2.1 채팅 욕 DB 거르기 에코 서버
  - 2.2 서버 IOCP, 스트림 큐이용 


- 기능
``` 
FriendList  - 친구목록 
FriendList_Request	- 친구목록_요청보낸거
FriendList_Reply	- 친구목록_요청받은거
FriendRemove  - 친구관계 끊기
FriendRequest  - 친구 요청하기
FriendCancel  - 친구 요청 보낸거 취소
FriendDeny  - 친구 요청 거부
FriendAgree  - 친구 요청 수락
```

- 친구 시스템 종류
```
팔로워방식(트위터)
신청즉시 팔로워가 됨
상대방의 정보를 볼 분 쌍방향 컨텐츠는 거의 없음
```

- 친구방식(페이스북)
```
친구요청>수락 or 거부
요청과 수락이라는 과정이 있으며
친구관게에서는 서로 컨텐츠를 공유하며 플레이
```

- 초대시스템
```
카카오톡처럼 특정 플렛폼에 붙을 경우
해당 플렛폼의 다른 유저들을 해당 게임으로 초대함
단순히 메시지를 보내는 초대라면 아주 간단하지만
일정시간 내에 초대 횟수 제한, 그리고 초대에대한 보상처리가 거의 들어감
```
- 개발 조건
1. 메시지 생성 . 무조건 직렬화버퍼를 사용
2. 수신 처리부 메시지 (패킷)하나당 하나의 처리함수 만들기
3. 송신 처리부


## 2D게임 - TCP 파이터 서버와 클라이언트 

- 용량이 커서 아직 올리지 못했습니다.
- [유튜브 데모 동영상](https://www.youtube.com/watch?v=hFowcbm8Xwo&feature=youtu.be)

<div>
<img src="https://github.com/heonilp/Game/blob/master/TCP%ED%8C%8C%EC%9D%B4%ED%84%B0%20%EC%84%9C%EB%B2%84%20%ED%81%B4%EB%9D%BC%EC%9D%B4%EC%96%B8%ED%8A%B8/%EC%8B%A4%ED%96%89.png" width="70%"></img>
</div>

-  TCP 파이터 서버와 클라이언트의 특징 및 구현

- 서버, 2D  클라이언트 모두 구현
- 언어 : C++/ C  , C++ STL 사용  
- 서버 모델 :Select 
- 클라이언트, 서버 섹터 구현
- 큐, 리스트 구현
- 구조체로 패킷 구현
- 객체 클래스화


## 보석 파이터 게임

- 용량이 커서 아직 올리지 못했습니다.
- [유튜브 데모 동영상](https://www.youtube.com/watch?v=13Un1BnTYoY&feature=youtu.be)

1.  유니티 액션 게임 게임서버 , 채팅서버  IOCP 이용,DB

<div>
<img src="https://github.com/heonilp/Game/blob/master/%EB%B3%B4%EC%84%9D%20%ED%8C%8C%EC%9D%B4%ED%84%B0%20%EA%B2%8C%EC%9E%84/1.png" width="70%"></img>
</div>


2.  MySQL DB 구조와 모니터링 클라이언트

<div>
<img src="https://github.com/heonilp/Game/blob/master/%EB%B3%B4%EC%84%9D%20%ED%8C%8C%EC%9D%B4%ED%84%B0%20%EA%B2%8C%EC%9E%84/2.png" width="70%"></img>
</div>

3. 채팅 서버와 게임 서버 구현

<div>
<img src="https://github.com/heonilp/Game/blob/master/%EB%B3%B4%EC%84%9D%20%ED%8C%8C%EC%9D%B4%ED%84%B0%20%EA%B2%8C%EC%9E%84/3.png" width="70%"></img>
</div>

- 상속 및 코딩스타일
<div>
<img src="https://github.com/heonilp/Game/blob/master/%EB%B3%B4%EC%84%9D%20%ED%8C%8C%EC%9D%B4%ED%84%B0%20%EA%B2%8C%EC%9E%84/3.png" width="70%"></img>
</div>

- 서버 : C/C++, IOCP, MySQL, List, 직렬화큐 등, 클라이언트 : 유니티 C# (게임코디 제공)

## 서버 모니터링 뷰어차트

- 서버 모니터링 뷰어차트 ([링크](https://github.com/heonilp/Game/tree/master/Win32%20API%20%EC%84%9C%EB%B2%84%20%EB%AA%A8%EB%8B%88%ED%84%B0%EB%A7%81%20%EB%B7%B0%EC%96%B4%EC%B0%A8%ED%8A%B8))
 
<div>
<img src="https://github.com/heonilp/Game/blob/master/Win32%20API%20%EC%84%9C%EB%B2%84%20%EB%AA%A8%EB%8B%88%ED%84%B0%EB%A7%81%20%EB%B7%B0%EC%96%B4%EC%B0%A8%ED%8A%B8/%EC%84%9C%EB%B2%84%20%EB%AA%A8%EB%8B%88%ED%84%B0%EB%A7%81%20%EB%B7%B0%EC%96%B4%EC%B0%A8%ED%8A%B8%20%EA%B2%B0%EA%B3%BC%20%ED%99%94%EB%A9%B4.png" width="70%"></img>
</div>


 1. C++ 윈도우 API 이용
 2. 큐 이용
 3. 서버 모니터링을 하기 위한 그래프 작업
 4. 일정 수준이 넘으면 소리나게 함
