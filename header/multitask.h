//multitask.h

#pragma once

/* --In lua file...

	f = function()
		while true do
			upvalue:method()  --Do something.
			coroutine.yield()
		end
	end

    --Create task from the function.
	--The task will start automatically.
	new_task(f) 

*/

/* --In host code...

	TaskSystem task_system(L); //L: main thread

	lua_resume(L, nullptr, 0); //Run main thread.

	task_system.resume_all();  //Run child threads.

*/

#include <list>
#include <boost/range/algorithm_ext/erase.hpp>
#include "common.h"

namespace MyGameProject
{
	namespace LuaUtilities
	{
		class TaskSystem
		{
		private:
			using ThreadRef = decltype(luaL_ref(nullptr, 0));
			lua_State* main_thread;
			static constexpr ThreadStatus LUA_READY{ LUA_YIELD };
			struct Task
			{
				ThreadRef thread;
				ThreadStatus status;
				bool to_continue(void) const { return status == LUA_YIELD || status == LUA_READY; }
			};
			using TaskList = std::list<Task>;
			TaskList task_list;
			static ThreadStatus resume_thread(lua_State* _parent, ThreadRef _ref_to_thread)
			{
				//Get thread object,
				lua_rawgeti(_parent, LUA_REGISTRYINDEX, _ref_to_thread);
				auto thread{ lua_tothread(_parent, -1) };

				//Pop the thread from stack.
				lua_pop(_parent, 1);

				//and call it.
				return lua_resume(thread, _parent, 0);
			}
			static int run_task(lua_State* _parent)
			{
				//coroutine.create(func)
				lua_getglobal(_parent, "coroutine");
				lua_getfield(_parent, -1, "create");
				lua_pushvalue(_parent, 1); //argument
				lua_pcall(_parent, 1, 1, 0);

				//co is reference to the thread object.
				auto co{ luaL_ref(_parent, LUA_REGISTRYINDEX) };

				//Register the coroutine.
				auto& task_list{ *static_cast<TaskList*>(lua_touserdata(_parent, lua_upvalueindex(1))) };
				task_list.push_back({ co, LUA_READY });

				return 0;
			}
		public:
			TaskSystem(void) :main_thread(nullptr), task_list() {}
			//Enable multitasking.
			TaskSystem(lua_State* _main_thread)
			{
				initialize(_main_thread);
			}
			//Enable multitasiking in the thread.
			void initialize(lua_State* _main_thread)
			{
				if (!_main_thread)
				{
					throw std::runtime_error("TaskSystem must be initialized by valid thread.");
				}

				if (!task_list.empty())
				{
					task_list.clear();
				}

				main_thread = _main_thread;
				lua_pushlightuserdata(main_thread, &task_list); //Task list as upvalue.
				lua_pushcclosure(main_thread, &run_task, 1);
				lua_setglobal(main_thread, "run_task");
			}
			//Resume all tasks.
			void resume_all(void)
			{
				//Remove all threads not resumable.
				boost::remove_erase_if(task_list, [](const Task& _t) {return !_t.to_continue(); });

				//Resume all thread on the list.
				for (auto& t : task_list)
				{
					t.status = resume_thread(main_thread, t.thread);
					//If the thread ended its life, disable reference to it(being target of Garbage collection).
					if (!t.to_continue()) { luaL_unref(main_thread, LUA_REGISTRYINDEX, t.thread); }
				}
			}
			//Return the number of the tasks.
			std::size_t count(void) const { return task_list.size(); }
		};

	}
}
