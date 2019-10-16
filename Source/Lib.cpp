#include <L2NextCrypto.h>
#include <nan.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using v8::Function;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Local;
using v8::Value;
using Nan::CopyBuffer;
using Nan::Utf8String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;
using Nan::CopyBuffer;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::To;
using Nan::Null;
using Nan::HandleScope;


class DecryptWorker: public AsyncWorker {
public:
	DecryptWorker(const Utf8String* data, const int size, Callback* callback) : AsyncWorker(callback), data(data), size(size) {};
	~DecryptWorker() {};

	void Execute() {
		//auto options = SBufStreamOptions{ **data, **data + size };
		//input = StreamFactory::Make<ReadableStream>(ReadableStream(options));
		//output = StreamFactory::Make<WritableStream>(WritableStream(options));
		auto fileData = SFileData(**data, 0);
		result = L2NextCrypto::Decrypt(fileData);
	};

	void HandleOKCallback() {
		HandleScope scope;

		const auto schemaObj = New<Object>();
		schemaObj->Set(New<String>("result").ToLocalChecked(), New<String>(result.data.content).ToLocalChecked());
		//schemaObj->Set(New<String>("errorMsg").ToLocalChecked(), New<String>(schema.errorMsg).ToLocalChecked());

		Local<Value> argv[] = { schemaObj };
		callback->Call(1, argv, async_resource);
	};

private:
	const Utf8String* data = nullptr;
	int size = 0;

	SDecryptResult result;
};

void Decrypt(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	const auto arg0 = info[0];
	const auto arg1 = info[1];
	const auto arg2 = info[2];

	const auto data = new Utf8String(To<String>(arg0).ToLocalChecked());
	const auto size = To<int>(arg1).FromJust();
	const auto callback = new Callback(To<Function>(arg2).ToLocalChecked());

	//cout << "len:" << data->length() << "\n";

	AsyncQueueWorker(new DecryptWorker(data, size, callback));
};

NAN_MODULE_INIT(InitAll) {
	Set(target, New<String>("decrypt").ToLocalChecked(), GetFunction(New<FunctionTemplate>(Decrypt)).ToLocalChecked());
};

NODE_MODULE(addon, InitAll);