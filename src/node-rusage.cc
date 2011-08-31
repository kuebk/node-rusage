/*
 * Copyright (C) 2011 by Jakub Lekstan <kuebzky@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <v8.h>
#include <node.h>
#include <sys/time.h>
#include <sys/resource.h>

int globalWho = RUSAGE_SELF;

static v8::Handle<v8::Value> get_r_usage(const v8::Arguments& args){
	v8::HandleScope scope;

	int localWho = globalWho;

	if(args.Length() != 0){
		bool isError = false;
		if(args[0]->IsNumber()){
			v8::Local<v8::Integer> iWho = v8::Local<v8::Integer>::Cast(args[0]);
			localWho = (int)(iWho->Int32Value());

			if(localWho != RUSAGE_SELF && localWho != RUSAGE_CHILDREN){
				isError = true;
			}
		}else{
			isError = true;
		}

		if(isError){
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("First argument must be either a RUSAGE_SELF or RUSAGE_CHILDREN")));
		}
	}

	rusage rusagedata;

	int status = getrusage(localWho, &rusagedata);

	if(status != 0){
		scope.Close(v8::Null());
	}

	v8::Local<v8::Object> data = v8::Object::New();

	data->Set(v8::String::New("ru_utime.tv_sec"), v8::Number::New(rusagedata.ru_utime.tv_sec));
	data->Set(v8::String::New("ru_utime.tv_usec"), v8::Number::New(rusagedata.ru_utime.tv_usec));

	data->Set(v8::String::New("ru_stime.tv_sec"), v8::Number::New(rusagedata.ru_stime.tv_sec));
	data->Set(v8::String::New("ru_stime.tv_usec"), v8::Number::New(rusagedata.ru_stime.tv_usec));
	
	data->Set(v8::String::New("ru_maxrss"), v8::Number::New(rusagedata.ru_maxrss));
	data->Set(v8::String::New("ru_ixrss"), v8::Number::New(rusagedata.ru_ixrss));
	data->Set(v8::String::New("ru_idrss"), v8::Number::New(rusagedata.ru_idrss));
	data->Set(v8::String::New("ru_isrss"), v8::Number::New(rusagedata.ru_isrss));
	data->Set(v8::String::New("ru_minflt"), v8::Number::New(rusagedata.ru_minflt));
	data->Set(v8::String::New("ru_majflt"), v8::Number::New(rusagedata.ru_majflt));
	data->Set(v8::String::New("ru_nswap"), v8::Number::New(rusagedata.ru_nswap));
	data->Set(v8::String::New("ru_inblock"), v8::Number::New(rusagedata.ru_inblock));
	data->Set(v8::String::New("ru_oublock"), v8::Number::New(rusagedata.ru_oublock));
	data->Set(v8::String::New("ru_msgsnd"), v8::Number::New(rusagedata.ru_msgsnd));
	data->Set(v8::String::New("ru_msgrcv"), v8::Number::New(rusagedata.ru_msgrcv));
	data->Set(v8::String::New("ru_nsignals"), v8::Number::New(rusagedata.ru_nsignals));
	data->Set(v8::String::New("ru_nvcsw"), v8::Number::New(rusagedata.ru_nvcsw));
	data->Set(v8::String::New("ru_nivcsw"), v8::Number::New(rusagedata.ru_nivcsw));

	return scope.Close(data);
}

static v8::Handle<v8::Value> usage_cycles(const v8::Arguments& args){
	v8::HandleScope scope;

	rusage rusagedata;

	int status = getrusage(globalWho, &rusagedata);

	if(status != 0){
		return scope.Close(v8::Null());
	}

	return scope.Close(v8::Number::New(rusagedata.ru_utime.tv_sec * 1e6 + rusagedata.ru_utime.tv_usec));
}

static v8::Handle<v8::Value> who(const v8::Arguments& args){
	v8::HandleScope scope;

	if(args.Length() != 0 && args[0]->IsNumber()){
		v8::Local<v8::Integer> iWho = v8::Local<v8::Integer>::Cast(args[0]);
		int localWho = (int)(iWho->Int32Value());

		if(localWho != RUSAGE_SELF && localWho != RUSAGE_CHILDREN){
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("First argument must be either a RUSAGE_SELF or RUSAGE_CHILDREN")));
		}

		globalWho = localWho;

		return scope.Close(v8::True());
	}else{
		return scope.Close(v8::False());
	}

}

extern "C" void init (v8::Handle<v8::Object> target){
	v8::HandleScope scope;

	NODE_SET_METHOD(target, "get", get_r_usage);
	NODE_SET_METHOD(target, "cycles", usage_cycles);
	NODE_SET_METHOD(target, "who", who);

	target->Set(v8::String::New("RUSAGE_SELF"), v8::Number::New(RUSAGE_SELF));
	target->Set(v8::String::New("RUSAGE_CHILDREN"), v8::Number::New(RUSAGE_CHILDREN));
}

