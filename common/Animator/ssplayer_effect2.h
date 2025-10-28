#ifndef __SSEFFECTRENDER_V3__
#define __SSEFFECTRENDER_V3__


#include "xorshift32.h"
#include "../../SS6Player.h"
#include "../loader/ssloader.h"
#include "ssplayer_cellmap.h"
#include "ssplayer_PartState.h"

//SsVector�Ȃ�
//#include "SsUtility.h"

//#include "ISSEffectRender.h"


namespace ss
{

class SsEffectModel;
class SsRenderEffectBase;
class SsEffectNode;
class SsPartState;
class SsEffectRenderAtom;
class SsCell;
class CustomSprite;


#define SEED_MAGIC (7573)
#define LIFE_EXTEND_SCALE (8)
#define LIFE_EXTEND_MIN	  (64)

#define LOOP_TYPE1 (0)
#define LOOP_TYPE2 (0)
#define LOOP_TYPE3 (1)


struct TimeAndValue
{
	float time;
	float value;
};


//v3.1
struct particleExistSt
{
	int  id;
	int	 cycle;
	int	 exist;
    int	 born;
	long stime;
	long endtime;
};


//v3.1
struct emitPattern
{
	int	  uid;
	int   life;
    int   cycle;
};


//�ŏI�`��p�f�[�^
struct particleDrawData
{
	int	id;
	int	pid;
	int	stime;		//�������ꂽ�J�n����
	int lifetime;

	//�`��p���[�N
	float	x;
	float	y;
	float	rot;
	float	direc;

    SsU8Color color;
    SsVector2 scale;
};


#if 0
//�����O�o�b�t�@�������͂�������Ȃ�����
template<class mytype>
class particleRingBuffer
{
protected:

	mytype*		ar;
	int*		realIndex;
	size_t		bufsize;


public:
	particleRingBuffer() {
    	 resize(16);
	}
	particleRingBuffer(size_t size)
	{
           resize(size);
	}
	virtual ~particleRingBuffer()
	{
		delete[] ar;
		delete[] realIndex;
	}

	void clear() {
		memset(ar, 0, sizeof(mytype) *(bufsize + 1));
		memset(realIndex, 0, sizeof(int) *(bufsize + 1));
	}

	void store(int index, mytype* t)
	{
		ar[index % bufsize] = *t;
		realIndex[index % bufsize] = index;
	}

	void resize( size_t size )
	{
		bufsize = size*2;
		ar = new mytype[bufsize +1];
		realIndex = new int[bufsize + 1];
		clear();
	}

	mytype& load(int index)
	{
		return ar[index % bufsize];
	}

	int loadRealIndex(int index)
	{
		return realIndex[index];
	}
	size_t	getBufsize() { return bufsize; }

};

#endif


//�G�~�b�^�[�����p�����[�^
//�G�f�B�^��t�@�C��������͂����
struct emitterParameter
{
	int	life;
	int interval;
	int	emitnum;		//��x�ɔr�o������
	int emitmax;		//�ő�r�o��
	int	particleLife;	//���������p�[�e�B�N���̎���
	int particleLife2;	//�����ő�l
    bool Infinite;      //��������

	int	loopStart;
	int loopEnd;
	int loopLen;
	int	loopGen;


	//�e�X�g�p�f�t�H���g
	emitterParameter() : life(15),interval(1),emitnum(2),emitmax(32),particleLife(15),particleLife2(15),Infinite(false){}

};


//�p�[�e�B�N�������p�����[�^
//�G�f�B�^��t�@�C��������͂����
struct particleParameter
{

	SsVector2 	scale;

	SsU8Color   startcolor; //�X�^�[�g���̃J���[
	SsU8Color   endcolor;   //�I�����̃J���[

	//����
	float		speed;		//����
	float		speed2;		//�����ő�l


	float		angle;		  //�����Ă�p�x
	float       angleVariance;//�ύ�

	bool		useGravity;
	SsVector2	gravity;


	bool		useOffset;
	SsVector2   offset;
	SsVector2   offset2;

	bool        useRotation;
	float		rotation;
	float		rotation2;

	float		rotationAdd;
	float		rotationAdd2;

	bool		useRotationTrans;
	float		rotationFactor;
	float		endLifeTimePer;

	bool        useTanAccel;
	float		tangentialAccel;
	float		tangentialAccel2;

	bool        useColor;
	SsU8Color   initColor;
	SsU8Color   initColor2;

	bool		useTransColor;
	SsU8Color   transColor;
	SsU8Color   transColor2;

	bool        useInitScale;
	SsVector2   scaleRange;
	float		scaleFactor;
	float		scaleFactor2;

	bool        useTransScale;
	SsVector2   transscale;
	SsVector2   transscaleRange;
	float		transscaleFactor;
	float		transscaleFactor2;

	float		delay;

	bool		usePGravity;
	SsVector2	gravityPos;
	float		gravityPower;

	bool		useAlphaFade;
	float       alphaFade;
	float       alphaFade2;

	bool		useTransSpeed;
	float	   	transSpeed;
	float	   	transSpeed2;

	bool		useTurnDirec;
    float		direcRotAdd;

    bool		userOverrideRSeed;
	int			overrideRSeed;


	particleParameter(){}
};


//�G�~�b�^����N���X
class SsEffectEmitter
{
public:
	SsCellValue		 dispCell;


	int					priority;

	emitterParameter  	emitter;
	particleParameter   particle;
	xorshift32			rand;


	int					emitterSeed;
	int					seedOffset;

	//�����p�̃����O�o�b�t�@
	TPL::StlVector<emitPattern>    	_emitpattern;
	TPL::StlVector<int>				_offsetPattern;

    particleExistSt*     particleExistList;


	//���O�v�Z�o�b�t�@
	//particleLifeSt*				particleList;
	int							particleIdMax;

	size_t						particleListBufferSize;
    unsigned long*              seedList;


	SsVector2   				position;
//	SsEffectEmitter*			_child;
	SsEffectEmitter*			_parent;

    int							_parentIndex;

	SsCell*						refCell;    //�`��p�Z��
	SsEffectBehavior*           refData;	//�f�[�^�X�V�p

	size_t						globaltime;
	size_t						seedTableLen;

	int							uid; 

public:
	SsEffectEmitter() :
//			particleList(0),
			_parentIndex(-1),
			seedList(0),
			particleListBufferSize(180*100),  //�����o����p�[�e�B�N���̍ő�l
			_emitpattern(0),
			particleExistList(0),
			globaltime(0),
			seedOffset(0)
	{
		emitterSeed = SEED_MAGIC;
	}
	virtual ~SsEffectEmitter()
	{
		delete[] particleExistList;
		delete[] seedList;

	}

	void	setSeedOffset(int offset) {
		seedOffset = offset;
	}

//	const particleLifeSt*	getParticleDataFromID(int id) { return &particleList[id]; }

#if  LOOP_TYPE3

	int	getParticleIDMax() { return _offsetPattern.size(); }
	const 	particleExistSt*	getParticleDataFromID(int id);
	void	updateEmitter(double time, int slide);

#else

	int	getParticleIDMax() { return particleIdMax; }
	const particleLifeSt*	getParticleDataFromID(int id);
#endif


	int	getTimeLength() { return emitter.life + ( emitter.particleLife + emitter.particleLife2); }

	//���ݎ��Ԃ���Y�o�����ʒu�����߂�
	//time�ϐ����狁�߂��鎮�Ƃ���
	void	updateParticle(float time, particleDrawData* p, bool recalc = false );

	//�p�[�e�B�N���̔����Ԋu�����O�v�Z����
	//�����ŏo�͂��m�肷��

	void	precalculate2();

};


class SsEffectRenderV2
{
public:

	//�G�t�F�N�g�̃p�����[�^�f�[�^
	SsEffectModel*		effectData;

	//Model�ɋL�ڂ���Ă���G�~�b�^�̃��X�g
	TPL::StlVector<SsEffectEmitter*>   emmiterList;

	TPL::StlVector<SsEffectEmitter*>   updateList;

	//�����_���V�[�h
	u32				mySeed;

	SsVector3		layoutPosition;
	SsVector2		layoutScale;

	float			nowFrame;
	float			targetFrame;
    float			secondNowFrame;

	size_t          effectTimeLength;

    bool			Infinite;	//�����ɔ����o���邩�ǂ���

	SsPartState*	parentState;

	bool			isIntFrame;

	bool			m_isPlay;
	bool			m_isPause;
	bool			m_isLoop;

	int				seedOffset;
	//	SsCellMapList*	curCellMapManager;/// �Z���}�b�v�̃��X�g�i�A�j���f�R�[�_�[������炤
	bool		_isWarningData;

	//�e�ɂȂ�X�v���C�g
	bool _isContentScaleFactorAuto;
	CustomSprite						*_parentSprite;

	int			_drawSpritecount; 

public:


protected:
	void 	particleDraw(SsEffectEmitter* e , double t , SsEffectEmitter* parent = 0 , particleDrawData* plp = 0 );
	void	initEmitter( SsEffectEmitter* e , SsEffectNode* node);

	void	clearEmitterList();



public:
	SsEffectRenderV2() : effectTimeLength(0), isIntFrame(true), seedOffset(0), mySeed(0), _parentSprite(0), _isContentScaleFactorAuto(false){}
	virtual ~SsEffectRenderV2() 
	{
		clearEmitterList();
	}

	virtual void    play(){ m_isPause = false;m_isPlay=true; }
	virtual void	stop(){ m_isPlay = false;}
	virtual void    pause(){m_isPause = true;m_isPlay=false;}
	virtual void	setLoop(bool flag){ m_isLoop = flag; }
	virtual bool	isplay(){return m_isPlay;}
	virtual bool	ispause(){return m_isPause;}
	virtual bool	isloop(){return m_isLoop;}

	virtual void	setEffectData(SsEffectModel* data);


	virtual void	setSeed( u32 seed )
	{
   		mySeed = seed * SEED_MAGIC;
	}


	virtual void	setFrame( float frame )
	{
    	nowFrame = frame;
	}

    virtual float	getFrame(){ return nowFrame; }

	virtual void	update();
	virtual void	draw();

	virtual void    reload();

    virtual size_t  getEffectTimeLength();

//	virtual int		getVersion(){ return EFFECTRENDERVERSION_V2; }

	virtual  void	setParentAnimeState( SsPartState* state ){ parentState = state; }

	virtual int	getCurrentFPS();

//	void	setCellmapManager( SsCellMapList* plist ) { curCellMapManager = plist; }

	bool	getPlayStatus(void){
		return(m_isPlay);
	}

	void	drawSprite(
			SsCellValue*		dispCell,
			SsVector2	_position,
			SsVector2 _size,
			float     _rotation,
			float	  direction,
			SsFColor	_color,
			SsRenderBlendType::_enum blendType
		);

	void	setSeedOffset(int offset) {
		if (effectData->isLockRandSeed)
		{
			seedOffset = 0;
		}
		else
		{
			seedOffset = offset;
		}
	}
	virtual bool	isInfinity() { return Infinite; }
	virtual bool	isWarning() { return _isWarningData; }

	//�e�ɂȂ�X�v���C�g��ݒ肷��
	void setContentScaleEneble(bool eneble) { _isContentScaleFactorAuto = eneble; }
	void setParentSprite(CustomSprite* sprite) { _parentSprite = sprite; }

	int	getDrawSpriteCount() { return _drawSpritecount; }
};

};

#endif