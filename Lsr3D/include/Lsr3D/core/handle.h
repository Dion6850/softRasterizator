/**
 * @file handle.h
 * @author zhywyt (zhywyt@yeah.net)
 * @brief copy from https://github.com/iGame-Lab/iGameView.git
 * @version 0.1
 * @date 2025-07-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#include <iostream>

namespace lsr3d {
	class Handle {
	public:
		// 显式构造，不允许 "iGameHandle h = 1;" 的语法进行隐式转换
		explicit Handle(int _idx) : idx_(_idx) {};
		// 赋值构造
		Handle& operator=(const Handle& _h) {
			idx_ = _h.idx_;
			return *this;
		}

		// 基类handle允许用int进行修改
		Handle& operator=(int _idx) {
			idx_ = _idx;
			return *this;
		}

		//判定handle是否存在
		inline bool is_valid() const { return idx_ != -1; }
		/*===========================================handle的比较操作===========================================*/
		inline bool operator<(const Handle& _h) const { return (this->idx_ < _h.idx_); }

		inline bool operator<(int _idx) const { return idx_ < _idx; }

		inline bool operator>(const Handle& _h) const { return (this->idx_ > _h.idx_); }

		inline bool operator>(int _idx) const { return idx_ > _idx; }

		inline bool operator==(const Handle& _h) const { return _h.idx_ == this->idx_; }

		inline bool operator!=(const Handle& _h) const { return _h.idx_ != this->idx_; }

		/*===========================================修改与重置===========================================*/

		inline const int& idx() const { return idx_; }      //取元素

		void idx(const int& _idx) { idx_ = _idx; }      //修改元素

		inline operator int() const { return idx_; } //隐式转换为int

		void reset() { idx_ = -1; }  //初始化

	private:
		int idx_;
	};

	//显式构造能够避免其他类型的handle赋值给当前类型的handle
	class VertexHandle : public Handle { public: explicit VertexHandle(int _idx = -1) : Handle(_idx) {} };
	class TextureCoordHandle : public Handle { public: explicit TextureCoordHandle(int _idx = -1) : Handle(_idx) {} };
	class TriangleHandle : public Handle { public: explicit TriangleHandle(int _idx = -1) : Handle(_idx) {} };
	class NormalHandle : public Handle { public: explicit NormalHandle(int _idx = -1) : Handle(_idx) {} };
	class MaterialHandle : public Handle { public: explicit MaterialHandle(int _idx = -1) : Handle(_idx) {} };
	class ImageHandle : public Handle { public: explicit ImageHandle(int _idx = -1) : Handle(_idx) {} };
}

/*======================特化各种iGameHandle的哈希映射============================*/
namespace std
{
	template<>
	struct hash<lsr3d::VertexHandle>
	{
		size_t operator()(const lsr3d::VertexHandle& h)const
		{
			return hash<int>()(h.idx());
		}
	};
	template<>
	struct hash<lsr3d::TextureCoordHandle>
	{
		size_t operator()(const lsr3d::TextureCoordHandle& h)const
		{
			return hash<int>()(h.idx());
		}
	};
	template<>
	struct hash<lsr3d::TriangleHandle>
	{
		size_t operator()(const lsr3d::TriangleHandle& h)const
		{
			return hash<int>()(h.idx());
		}
	};
	template<>
	struct hash<lsr3d::NormalHandle>
	{
		size_t operator()(const lsr3d::NormalHandle& h)const
		{
			return hash<int>()(h.idx());
		}
	};
	template<>
	struct hash<lsr3d::MaterialHandle>
	{
		size_t operator()(const lsr3d::MaterialHandle& h)const
		{
			return hash<int>()(h.idx());
		}
	};
	template<>
	struct hash<lsr3d::ImageHandle>
	{
		size_t operator()(const lsr3d::ImageHandle& h)const
		{
			return hash<int>()(h.idx());
		}
	};

}



// note:
// 派生类如果想调用基类已被覆盖的操作符
// 例如 "iGameHandle& operator=(int _idx)"
//void test() {
//	iGameVertexHandle vh(1);
//	vh = 2;                      错误，不允许隐式转换
//	vh.iGameHandle::operator= (2);    正确，调用基类操作符
//}
