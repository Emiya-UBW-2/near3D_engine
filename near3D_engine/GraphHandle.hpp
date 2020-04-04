#pragma once
#include <DxLib.h>
#include <vector>
#include <string_view>
class GraphHandle {
private:
	int handle_;
	constexpr GraphHandle(int h) noexcept : handle_(h) {}
	static constexpr int invalid_handle = -1;

public:
	constexpr GraphHandle() noexcept : handle_(invalid_handle) {}
	GraphHandle(const GraphHandle&) = delete;
	GraphHandle(GraphHandle&& o) noexcept : handle_(o.handle_) {
		o.handle_ = invalid_handle;
	}
	GraphHandle& operator=(const GraphHandle&) = delete;
	GraphHandle& operator=(GraphHandle&& o) noexcept {
		this->handle_ = o.handle_;
		o.handle_ = invalid_handle;
		return *this;
	}
	~GraphHandle() noexcept {
		if (-1 != this->handle_) {
			DeleteGraph(this->handle_);
		}
	}
	void Dispose() noexcept {
		if (-1 != this->handle_) {
			DeleteGraph(this->handle_);
			this->handle_ = -1;
		}
	}
	int get() const noexcept { return handle_; }
	auto getp() noexcept { return &handle_; }
	static GraphHandle Load(std::basic_string_view<TCHAR> FileName, bool NotUse3DFlag = false) noexcept {
		return { DxLib::LoadGraphWithStrLen(FileName.data(), FileName.length(), NotUse3DFlag) };
	}
	static void LoadDiv(std::basic_string_view<TCHAR> FileName, int AllNum, int XNum, int YNum, int   XSize, int   YSize, std::vector<GraphHandle> *Handles, bool NotUse3DFlag = false) noexcept {
		int* HandleArray = new int[AllNum];
		DxLib::LoadDivGraphWithStrLen(FileName.data(), FileName.length(), AllNum, XNum, YNum, XSize, YSize, HandleArray, NotUse3DFlag);

		Handles->clear();
		for (int i = 0; i < AllNum; i++) {
			Handles->resize(Handles->size()+1);
			Handles->back() = HandleArray[i];
		}
		delete[] HandleArray;

		return;
	}
		
	static GraphHandle Make(int SizeX, int SizeY, bool UseAlphaChannel = false) noexcept {
		return { DxLib::MakeScreen(SizeX, SizeY, UseAlphaChannel) };
	}
};
