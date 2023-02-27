//
// Created by lcy on 21-6-15.
//

#ifndef ROBOT_MATLAB_RWLOCK_H
#define ROBOT_MATLAB_RWLOCK_H

#include <cstdlib>
#include <cassert>
#include <atomic>
#include <thread>
#include <type_traits>
#include <functional>

using namespace std;
//using namespace gyd;

namespace gyd {
    template<typename T>
    struct no_const {
        using type=typename std::conditional<std::is_const<T>::value,typename std::remove_const<T>::type,T>::type;
    };

    class raii{
    public:
        using fun_type =std::function<void()>;
        explicit raii(fun_type release, fun_type acquire = [] {}, bool default_com = true) noexcept:
                _commit(default_com), _release(release) {
            acquire();
        }

        ~raii() noexcept{
            if (_commit)
                _release();
        }
        raii(raii&& rv)noexcept:_commit(rv._commit),_release(rv._release){
            rv._commit=false;
        };

        raii(const raii&) = delete;
        raii& operator=(const raii&) = delete;
        raii& commit(bool c = true)noexcept { _commit = c; return *this; };
    private:
        bool _commit;
    protected:
        std::function<void()> _release;
    };

    template<typename T>
    class raii_var{
    public:
        using    _Self      = raii_var<T>;
        using   acq_type    =std::function<T()>;
        using   rel_type    =std::function<void(T &)>;
        explicit raii_var(acq_type acquire , rel_type release) noexcept:
                _resource(acquire()),_release(release) {
        }
        raii_var(raii_var&& rv):
                _resource(std::move(rv._resource)),
                _release(std::move(rv._release))
        {
            rv._commit=false;
        }

        ~raii_var() noexcept{
            if (_commit)
                _release(_resource);
        }
        _Self& commit(bool c = true)noexcept { _commit = c; return *this; };
        T& get() noexcept{return _resource;}
        T& operator*() noexcept
        { return get();}
        template<typename _T=T>
        typename std::enable_if<std::is_pointer<_T>::value,_T>::type operator->() const noexcept
        { return _resource;}
        template<typename _T=T>
        typename std::enable_if<std::is_class<_T>::value,_T*>::type operator->() const noexcept
        { return std::addressof(_resource);}

    private:
        bool    _commit=true;
        T   _resource;
        rel_type _release;
    };

    template<typename RES, typename M_REL, typename M_ACQ>
    raii make_raii(RES & res, M_REL rel, M_ACQ acq, bool default_com = true)noexcept {
        static_assert(std::is_class<RES>::value, "RES is not a class or struct type.");
        static_assert(std::is_member_function_pointer<M_REL>::value, "M_REL is not a member function.");
        static_assert(std::is_member_function_pointer<M_ACQ>::value,"M_ACQ is not a member function.");
        assert(nullptr!=rel&&nullptr!=acq);
        auto p_res=std::addressof(const_cast<typename no_const<RES>::type&>(res));
        return raii(std::bind(rel, p_res), std::bind(acq, p_res), default_com);
    }

    template<typename RES, typename M_REL>
    raii make_raii(RES & res, M_REL rel, bool default_com = true)noexcept {
        static_assert(std::is_class<RES>::value, "RES is not a class or struct type.");
        static_assert(std::is_member_function_pointer<M_REL>::value, "M_REL is not a member function.");
        assert(nullptr!=rel);
        auto p_res=std::addressof(const_cast<typename no_const<RES>::type&>(res));
        return raii(std::bind(rel, p_res), []{}, default_com);
    }
}

class RWLock {
#define WRITE_LOCK_STATUS -1
#define FREE_STATUS 0
private:
    static const  std::thread::id NULL_THEAD;
    const bool WRITE_FIRST;
    thread::id m_write_thread_id;
    atomic_int m_lockCount;
    atomic_uint m_writeWaitCount;
public:
    RWLock(const RWLock&) = delete;
    RWLock& operator=(const RWLock&) = delete;
    //RWLock& operator=(const RWLock&) volatile = delete;
    RWLock(bool writeFirst=false);
    virtual ~RWLock()=default;
    int readLock();
    int readUnlock();
    int writeLock();
    int writeUnlock();
    gyd::raii read_guard() const noexcept {
        return gyd::make_raii(*this,&RWLock::readUnlock,&RWLock::readLock);
    }
    gyd::raii write_guard() noexcept {
        return gyd::make_raii(*this,&RWLock::writeUnlock,&RWLock::writeLock);
    }
};

#endif //ROBOT_RWLOCK_H
