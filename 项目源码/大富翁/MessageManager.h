/*
 * MessageManager��
 * ���ߣ�X_Jun
 *
 * ���£�
 * 2017-8-26����������ĸı�������߼��㣬�����GetState()����
 *            ����ȷ����Ϸ�Ƿ���Կ�ʼ          -By X_Jun
 * 2017-8-28: ��ӽӿ��Է����ѯ�Ƿ��յ���Ϣ    -By X_Jun
 *
 * �������ڣ�2017-8-26
 * ������
 *     ����Ϣ���������ǿͻ��˵ĺ��Ĳ��֣������շ�������Ϣ��
 * Ȼ�������/���������߽��е��ȡ�
 *     Ҫ����MessageManager�࣬����Ҫ�������²�����
 *     �ٻ�ȡһ����Ϣ�������ࣺ���þ�̬����
 * MessageManager::GetMessageManager();  ����ȡһ����Ϣ����
 * ��������
 *     �ڳ�ʼ�������÷���MessageManager::Initialize(); 
 * ���������/�ֵ��������ָ�룬����ȡIP��Ϣ
 *     �ۼ��뷿�䣺����������÷���MessageManager::HostConnect();
 * ���������˵��������Ϣ
 *     ���ӷ�������÷���MessageManager::PlayerConnect();
 * ���������˵�����뷿��
 *     �ܸ��£���GameApp::Update()�е��÷���
 * MessageManager::Update(); ����ȡ��������Ϣ��������/�ֵ�
 * ����в���
 *     ���ж���Ϸ�Ƿ�ʼ��ʹ��MessageManager::GetState();���ж�
 * ��Ϸ�Ƿ�ʼ(ֵΪ1)
 *     ����GameApp::~GameApp()�е���
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
		 * ʹ��/ipAddr.txt�ļ�����ȡIP��ַ��Ϣ����ʼ��Winsock
		 * �ļ���ʽ��
		 * IP��ַ��XXX.XXX.XXX.XXX
		 * ͬʱ��ȡ��������ߺ���Ϣ������
		 */
		void Initialize(LPCSTR ipFileName, Rich::PlayerManager* playerManager, Rich::StreetManager* streetManager);
		/*
		* �������ӵ�����ˣ�����������䲢�趨�ó�ʼ������
		* �ṩ������������ʼ�ʽ����ʼ��ȯ
		*/
		void HostConnect(int numOfPlayers, int startGold, int startSliver);
		// ������ӵ�����ˣ�������뷿���
		void PlayerConnect();

		// ������Ϣ��ȷ��message�����Ԫ��������16��������
		void Send(int *message);
		
		// ��̨����
		void Detach() { m_tUpdate.detach(); }
		// �Ƿ��յ���Ϣ
		bool IsRecived() const { return m_Recived; }

		// ��ȡ״̬
		int GetState() const;

		// ��ȡ��Ϣ������
		static MessageManager* GetMessageManager();
		// �ͷ���Դ
		static void Release();
	private:
		MessageManager();
		~MessageManager();

		// ������Ϣ��������Ϣ������Ϣ�����Ӷ��ַ��������ã�
		void Update();

		SOCKET					m_Socket;				// �׽���
		sockaddr_in				m_ServAddr;				// ������IP��ַ
		int                     m_Buffer[16];			// ������

		int						m_State;				// ״̬

		bool                    m_Recived;				// ��ǰ֡�յ���Ϣ

		PlayerManager*			m_pPlayerManager;		// ��ҹ�����
		StreetManager*			m_pStreetManager;		// ����������

		static MessageManager*	m_pMessageManager;		// ��̬��Ϣ������

		std::thread				m_tUpdate;				// �̣߳�������
	};
}




#endif