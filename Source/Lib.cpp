#include <L2NextCrypto.h>
#include <nan.h>

using v8::Function;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Local;
using v8::Value;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::To;
using Nan::Null;
using Nan::HandleScope;


class DecryptWorker : public AsyncWorker {
public:
	DecryptWorker(Callback* callback, Local<String> filepath)
		: AsyncWorker(callback), filepath(filepath), cbresult(New<String>("Default").ToLocalChecked()) {}
	~DecryptWorker() {}

	// Executed inside the worker-thread.
	// It is not safe to access V8, or V8 data structures
	// here, so everything we need for input and output
	// should go on `this`.
	void Execute() {
		cbresult = New<String>("Welcome from callback").ToLocalChecked();
	}

	// Executed when the async work is complete
	// this function will be run inside the main event loop
	// so it is safe to use V8 again
	void HandleOKCallback() {
		HandleScope scope;

		Local<Value> argv[] = {
			Null()
		  , cbresult
		};

		callback->Call(2, argv, async_resource);
	}

private:
	Local<String> filepath;
	Local<String> cbresult;
};

void Decrypt(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Local<String> filepath = To<String>(info[0]).ToLocalChecked();

	Callback* callback = new Callback(To<Function>(info[1]).ToLocalChecked());

	AsyncQueueWorker(new DecryptWorker(callback, filepath));
}

NAN_MODULE_INIT(InitAll) {
	Set(target, New<String>("decrypt").ToLocalChecked(), GetFunction(New<FunctionTemplate>(Decrypt)).ToLocalChecked());
}

NODE_MODULE(addon, InitAll);