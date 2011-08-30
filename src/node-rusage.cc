#include <v8.h>
#include <node.h>
#include <sys/time.h>
#include <sys/resource.h>

int who = 0;

static v8::Handle<v8::Value> get_r_usage(const v8::Arguments& args){
	v8::HandleScope scope;

	if(args.Length() != 0){
		if(args[0]->IsNumber()){
			v8::Local<v8::Integer> iWho = v8::Local<v8::Integer>::Cast(args[0]);
			who = (int)(iWho->Int32Value());
		}else{
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("First argument must be a number")));
		}
	}

	rusage rusagedata;

	int status = getrusage(who, &rusagedata);

	if(status != 0){
		scope.Close(v8::Undefined());
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

	int status = getrusage(who, &rusagedata);

	if(status != 0){
		return scope.Close(v8::Undefined());
	}

	return scope.Close(v8::Number::New(rusagedata.ru_utime.tv_sec * 1e6 + rusagedata.ru_utime.tv_usec));
}

extern "C" void init (v8::Handle<v8::Object> target){
	v8::HandleScope scope;

	NODE_SET_METHOD(target, "get", get_r_usage);
	NODE_SET_METHOD(target, "cycles", usage_cycles);
}

