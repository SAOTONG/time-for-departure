//=================================================================================
//������D3DUtil.hͷ�ļ������ڹ���������Ķ���
//=================================================================================
#ifndef HR
//�Զ���һ��HR�꣬����ִ�д��󷵻�
#define HR(x) {hr = x;if(FAILED(hr)){return hr;}}
#endif // !HR

#ifndef SAFE_DELETE
//�Զ���һ��SAFE_DELETE�꣬����ָ����Դ��ɾ��
#define SAFE_DELETE(p) {if(p){delete(p);(p) = NULL;}}
#endif // !SAFE_DELETE

#ifndef SAFE_RELEASE
//�Զ���һ��SAFE_RELEASE�꣬����COM��Դ���ͷ�
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p) = NULL;}}
#endif // !SAFE_RELEASE

