//-------------------------------------------------------------------
//�w�i�̕\��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Board.h"

#include "task_Effect1.h"
#include "task_Effect2.h"
#include "easing.h"
#include "Task_Title.h"

namespace  Board
{
  Resource::WP  Resource::instance;
  //-------------------------------------------------------------------
  //���\�[�X�̏�����
  bool  Resource::Initialize()
  {
    this->imageName = "Card";
    DG::Image_Create(this->imageName, "./data/image/Card.png");
    return true;
  }
  //-------------------------------------------------------------------
  //���\�[�X�̉��
  bool  Resource::Finalize()
  {
    return true;
  }
  //-------------------------------------------------------------------
  //�u�������v�^�X�N�������ɂP�񂾂��s������
  bool  Object::Initialize()
  {
    //�X�[�p�[�N���X������
    __super::Initialize(defGroupName, defName, true);
    //���\�[�X�N���X����or���\�[�X���L
    this->res = Resource::Create();

    //���f�[�^������
    this->render2D_Priority[1] = 0.9f;

    //���^�X�N�̐���
    for (int i = 0; i < 7; i++) {
      chip[i] = ML::Box2D(i * 300, 0, 300, 400);
    }
    //�K�萔�̃J�[�h��u������Ƀ����_���ŃV���b�t������B
    int table[] = {4,4,4,2,2,2};
    int t = 0;
    for (int j = 0; j < 6; j++) {
      for (int i = 0; i < table[j]; i++) {
        GameBoard[t / 6][t % 6] = j;
        t++;
      }
    }
    //  ��ԊǗ��z�����ɂ��Ԃ��J�[�h�𗠕Ԃ�����Ԃɂ���B
    for (int y = 0; y < 3; y++) {
      for (int x = 0; x < 6; x++) {
        CheckBoard[y][x] = 1;
      }
    }
    //  �J�[�\���ʒu���O�C�O�̃J�[�h�ɍ��킹��B
    pos = { 0,0 };
    count = 0;
    clearFlag = false;

    return  true;
  }
  //-------------------------------------------------------------------
  //�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
  bool  Object::Finalize()
  {
    //���f�[�^���^�X�N���
    if (!ge->QuitFlag() && this->nextTaskCreate) {
      //�������p���^�X�N�̐���
      Title::Object::Create(true);
    }

    return  true;
  }

  bool Object::checkCorrect(int no1, int no2)
  {
    if (GameBoard[no1 / 6][no1 % 6] ==
      GameBoard[no2 / 6][no2 % 6]) {
      return true;
    }
    return false;
  }
  void Object::setOpenCard(int no)
  {
    CheckBoard[no / 6][no % 6] = 0;
  }
  void Object::setOpenCard(int x,int y)
  {
    CheckBoard[y][x] = 0;
  }

  void Object::setCloseCard(int no)
  {
    CheckBoard[no / 6][no % 6] = 1;
  }
  void Object::setCloseCard(int x, int y)
  {
    CheckBoard[y][x] = 1;
  }

  void Object::setGetCard(int no)
  {
    CheckBoard[no / 6][no % 6] = 2;
  }

  void Object::setGetCard(int x, int y)
  {
    CheckBoard[y][x] = 2;
  }

  bool Object::checkClear()
  {
    for (int y = 0; y < 3; y++) {
      for (int x = 0; x < 6; x++) {
        if (CheckBoard[y][x] != 2)
          return false;
      }
    }
    return true;
  }
  //-------------------------------------------------------------------
  //�u�X�V�v�P�t���[�����ɍs������
  void  Object::UpDate()
  {
    auto in = DI::GPad_GetState("P1");

    easing::UpDate();

    if (in.LStick.D.down) {
      easing::Create("curMoveY", easing::EASINGTYPE::CIRCOUT, (float)(pos.y*(200 + 10)), (float)((pos.y + 1)*(200 + 10)), 30);
      easing::Start("curMoveY");
      pos.y += 1;
    }
    if (in.LStick.U.down) {
      easing::Create("curMoveY", easing::EASINGTYPE::CIRCOUT, (float)(pos.y*(200 + 10)), (float)((pos.y - 1)*(200 + 10)), 30);
      easing::Start("curMoveY");
      pos.y -= 1;
    }
    if (in.LStick.L.down) {
      easing::Create("curMoveX", easing::EASINGTYPE::CIRCOUT, (float)(pos.x*(150 + 10)), (float)((pos.x - 1)*(150 + 10)), 30);
      easing::Start("curMoveX");
      pos.x -= 1;
    }
    if (in.LStick.R.down) {
      easing::Create("curMoveX", easing::EASINGTYPE::CIRCOUT, (float)(pos.x*(150 + 10)), (float)((pos.x + 1)*(150 + 10)), 30);
      easing::Start("curMoveX");
      pos.x += 1;
    }
    if (in.B1.down) {
      if (clearFlag) {
        this->Kill();
      }
      switch (count) {
      case 0:case 1:
        if (CheckBoard[pos.y][pos.x] == 1) {
          setOpenCard(pos.x, pos.y);
          //�߂���G�t�F�N�g�o��
          auto e = Effect1::Object::Create(true);
          e->pos.x = (float)pos.x;
          e->pos.y = (float)pos.y;

          //�ʂ��ԍ��ۑ�
          holdCard[count] = pos.y * 6 + pos.x;
          count++;
        }
        break;

      case 2:
        //���𔻒�
        if (checkCorrect(holdCard[0], holdCard[1])) {
          //�߂����Ă���J�[�h���J�����ςȂ���
          setGetCard(holdCard[0]);
          setGetCard(holdCard[1]);

          //�N���A�`�F�b�N
          if (checkClear()) {
            //�N���A�G�t�F�N�g�o��
            auto e = Effect2::Object::Create(true);
            e->pos.x = 1024 / 2;
            e->pos.y = 768 / 2;
            e->type = 2;
            clearFlag = true;
          }
          else {
            //�����G�t�F�N�g�o��
            auto e = Effect2::Object::Create(true);
            e->pos.x = 1024 / 2;
            e->pos.y = 768 / 2;
            e->type = 0;
          }
        }
        else {
          setCloseCard(holdCard[0]);
          setCloseCard(holdCard[1]);

          //�߂���G�t�F�N�g�o��
          auto e1 = Effect1::Object::Create(true);
          e1->pos.x = (float)(holdCard[0]%6);
          e1->pos.y = (float)(holdCard[0]/6);

          auto e2 = Effect1::Object::Create(true);
          e2->pos.x = (float)(holdCard[1]%6);
          e2->pos.y = (float)(holdCard[1]/6);
          //�s�����G�t�F�N�g�o��
          //�߂���G�t�F�N�g�o��
          auto e = Effect2::Object::Create(true);
          e->pos.x = 1024 / 2;
          e->pos.y = 768 / 2;
          e->type = 1;
        }
        count = 0;
        break;
      }
    }

  }
  //-------------------------------------------------------------------
  //�u�Q�c�`��v�P�t���[�����ɍs������
  void  Object::Render2D_AF()
  {
    //  ��ԊǗ��z�����ɂ��Ԃ��J�[�h�𗠕Ԃ�����Ԃɂ���B
    for (int y = 0; y < 3; y++) {
      for (int x = 0; x < 6; x++) {
        ML::Box2D  draw(x*(150+10), y*(200+10), 150, 200);
        ML::Box2D  src = chip[GameBoard[y][x]];
        DG::Image_Draw(this->res->imageName, draw, src);

        if (CheckBoard[y][x] == 1) {
          ML::Box2D  draw(x*(150 + 10), y*(200 + 10), 150, 200);
          ML::Box2D  src = chip[6];
          DG::Image_Draw(this->res->imageName, draw, src);
        }
      }
    }
    //�J�[�\���̕`��
    if (easing::GetState("curMoveX") == easing::EQ_STATE::EQ_END &&
      easing::GetState("curMoveY") == easing::EQ_STATE::EQ_END) {
      ML::Box2D  draw(pos.x*(150 + 10), pos.y*(200 + 10), 150, 200);
      ML::Box2D  src(300 * 6, 400, 300, 400);
      DG::Image_Draw(this->res->imageName, draw, src);
    }
    else if(easing::GetState("curMoveX") != easing::EQ_STATE::EQ_END){
      ML::Box2D  draw((int)easing::GetPos("curMoveX"), pos.y*(200 + 10), 150, 200);
      ML::Box2D  src(300 * 6, 400, 300, 400);
      DG::Image_Draw(this->res->imageName, draw, src);
    }
    else if (easing::GetState("curMoveY") != easing::EQ_STATE::EQ_END) {
      ML::Box2D  draw(pos.x*(150 + 10), (int)easing::GetPos("curMoveY"), 150, 200);
      ML::Box2D  src(300 * 6, 400, 300, 400);
      DG::Image_Draw(this->res->imageName, draw, src);
    }
  }

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}