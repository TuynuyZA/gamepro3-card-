#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�w�i�̕\��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Board
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Q�[����");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		string  imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
    ML::Box2D chip[10];
    //�Q�[���p2�����z��@�U���R
    int GameBoard[3][6];
    //�߂�������ԊǗ��z��@���Ԃ��A�߂������΂���A�����ɂ��߂���Ă���̂R�@����
    int CheckBoard[3][6];
    //�i�o��p�e�[�u���W���j�@���ざ�W
    //�J�[�\���ʒu�ϐ�
    POINT pos;
    //�c�胉�C�t
    int life;
    //����߂��������J�E���^
    int count;
    //�߂������J�[�h�̒ʂ��ԍ��z��@�Q�v�f
    int holdCard[2];
    //�N���A�t���O
    bool clearFlag;
    //�����`�F�b�N�֐�
    bool checkCorrect(int no1,int no2);
    void setOpenCard(int no);
    void setOpenCard(int x, int y);
    void setCloseCard(int no);
    void setCloseCard(int x, int y);
    void setGetCard(int no);
    void setGetCard(int x, int y);
    bool checkClear();
	};
}