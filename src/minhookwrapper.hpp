#pragma once
#include <MinHook.h>

#include <string>
#include <memory>
#include <functional>

template <typename FunctionPtrT>
class CHook 
{
public:
    CHook(std::uintptr_t target = 0) : _target(target)
    {
        MH_Initialize();
    }

    ~CHook()
    {
        this->Remove();
    }

    const char* Status()
    {
        return MH_StatusToString(_create_status);
    }

    bool Add(FunctionPtrT detour, bool enable = true) 
    {
        _create_status = MH_CreateHook(reinterpret_cast<void*>(_target), detour, reinterpret_cast<void**>(&_original));
        if (!enable)
        {
            return _create_status == MH_OK;
        }

        auto enabled = MH_EnableHook(reinterpret_cast<void*>(_target));
        return _create_status == MH_OK && enabled == MH_OK;
    }

    void SetAddr(std::uintptr_t target) 
    {
        _target = target;
    }

    bool Enable() 
    {
        return MH_EnableHook(reinterpret_cast<void *>(_target)) == MH_OK;
    }

    FunctionPtrT GetOriginal()
    {
        return _original;
    }

    bool Disable() 
    {
        return MH_DisableHook(reinterpret_cast<void*>(_target)) == MH_OK;
    }

    bool Remove() 
    {
        return MH_RemoveHook(reinterpret_cast<void*>(_target)) == MH_OK;
    }

    template<typename... Args>
    auto CallOriginal(Args&&... args) const 
    {
        return (*_original)(std::forward<Args>(args)...);
    }

private:
    MH_STATUS _create_status { MH_UNKNOWN };
    std::uintptr_t _target { 0 };
    void* _detour { nullptr };
    FunctionPtrT _original { nullptr };
};