//=================================================================================
//描述：D3DUtil.h头文件，用于公共辅助宏的定义
//=================================================================================
#ifndef HR
//自定义一个HR宏，方便执行错误返回
#define HR(x) {hr = x;if(FAILED(hr)){return hr;}}
#endif // !HR

#ifndef SAFE_DELETE
//自定义一个SAFE_DELETE宏，用于指针资源的删除
#define SAFE_DELETE(p) {if(p){delete(p);(p) = NULL;}}
#endif // !SAFE_DELETE

#ifndef SAFE_RELEASE
//自定义一个SAFE_RELEASE宏，用于COM资源的释放
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p) = NULL;}}
#endif // !SAFE_RELEASE

