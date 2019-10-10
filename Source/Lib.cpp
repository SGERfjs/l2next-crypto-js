#include "Crypto/Crypto.h"
#include <nan.h>

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::New;
using Nan::New;
using Nan::New;
using Nan::New;
using Nan::New;
using Nan::Set;

NAN_METHOD(SaySync) {
	info.GetReturnValue().Set(Nan::New("You see this!").ToLocalChecked());
}

NAN_MODULE_INIT(InitAll) {
	Set(target, New<String>("say").ToLocalChecked(), GetFunction(New<FunctionTemplate>(SaySync)).ToLocalChecked());
}

NODE_MODULE(addon, InitAll);