/*
 * MessageManager类
 * 作者：X_Jun
 *
 * 更新：
 * 2017-8-26：随人物类的改变更新了逻辑层，添加了GetState()函数
 *            用来确定游戏是否可以开始          -By X_Jun
 * 2017-8-28: 添加接口以方便查询是否收到消息    -By X_Jun
 *
 * 创建日期：2017-8-26
 * 详述：
 *     该消息管理者类是客户端的核心部分，负责收发核心消息，
 * 然后对人物/街区管理者进行调度。
 *     要是有MessageManager类，首先要进行如下操作：
 *     ①获取一个消息管理者类：调用静态方法
 * MessageManager::GetMessageManager();  来获取一个消息管理
 * 单例对象。
 *     ②初始化：调用方法MessageManager::Initialize(); 
 * 来获得人物/街道管理类的指针，并读取IP信息
 *     ③加入房间：开房者需调用方法MessageManager::HostConnect();
 * 来向服务器说明房间信息
 *     而加房者需调用方法MessageManager::PlayerConnect();
 * 来向服务器说明加入房间
 *     ④更新：在GameApp::Update()中调用方法
 * MessageManager::Update(); 来获取服务器消息并对人物/街道
 * 类进行操作
 *     ⑤判断游戏是否开始：使用MessageManager::GetState();来判断
 * 游戏是否开始(值为1)
 *     ⑥在GameApp::~GameApp()中调用
 * ReleaseStatic(MessageManager);
 */


#ifndef _MESSAGEMANAGER_H_
#define _MESSAGEMANAGER_H_

#include <Winsock2.h>
#include <fstream>
#include <thread>
#include "PlayerManager.h"
#include "StreetManager.h"


namespace Rich {
	class MessageManager {
	public:

		/*
		 * 使用/ipAddr.txt文件来读取IP地址信息，初始化Winsock
		 * 文件格式：
		 * IP地址：XXX.XXX.XXX.XXX
		 * 同时获取人物管理者和消息管理者
		 */
		void Initialize(LPCSTR ipFileName, Rich::PlayerManager* playerManager, Rich::StreetManager* streetManager);
		/*
		* 房主连接到服务端（点击创建房间并设定好初始参数后）
		* 提供创房人数，起始资金和起始点券
		*/
		void HostConnect(int numOfPlayers, int startGold, int startSliver);
		// 玩家连接到服务端（点击加入房间后）
		void PlayerConnect();

		// 发送消息，确保message数组的元素数大于16个！！！
		void Send(int *message);
		
		// 后台更新
		void Detach() { m_tUpdate.detach(); }
		// 是否收到消息
		bool IsRecived() const { return m_Recived; }

		// 获取状态
		int GetState() const;

		// 获取消息管理者
		static MessageManager* GetMessageManager();
		// 释放资源
		static void Release();
	private:
		MessageManager();
		~MessageManager();

		// 更新消息（接受消息并对消息处理，从而分发函数调用）
		void Update();

		SOCKET					m_Socket;				// 套接字
		sockaddr_in				m_ServAddr;				// 服务器IP地址
		int                     m_Buffer[16];			// 缓冲区

		int						m_State;				// 状态

		bool                    m_Recived;				// 当前帧收到消息

		PlayerManager*			m_pPlayerManager;		// 玩家管理者
		StreetManager*			m_pStreetManager;		// 街区管理者

		static MessageManager*	m_pMessageManager;		// 静态消息管理者

		std::thread				m_tUpdate;				// 线程：更新用
	};
}




#endif