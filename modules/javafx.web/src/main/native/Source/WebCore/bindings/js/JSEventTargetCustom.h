/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "DOMWindow.h"
#include "JSDOMBinding.h"
#include "JSDOMBindingSecurity.h"
#include "JSDOMOperation.h"

namespace WebCore {

// Wrapper type for JSEventTarget's castedThis because JSDOMWindow and JSWorkerGlobalScope do not inherit JSEventTarget.
class JSEventTargetWrapper {
    WTF_MAKE_FAST_ALLOCATED;
public:
    JSEventTargetWrapper(EventTarget& wrapped, JSC::JSObject& wrapper)
        : m_wrapped(wrapped)
        , m_wrapper(wrapper)
    { }

    EventTarget& wrapped() { return m_wrapped; }

    operator JSC::JSObject&() { return m_wrapper; }

private:
    EventTarget& m_wrapped;
    JSC::JSObject& m_wrapper;
};

std::unique_ptr<JSEventTargetWrapper> jsEventTargetCast(JSC::VM&, JSC::JSValue thisValue);

template<> class IDLOperation<JSEventTarget> {
public:
    using ClassParameter = JSEventTargetWrapper*;
    using Operation = JSC::EncodedJSValue(JSC::ExecState*, ClassParameter, JSC::ThrowScope&);

    template<Operation operation, CastedThisErrorBehavior shouldThrow = CastedThisErrorBehavior::Throw>
    static JSC::EncodedJSValue call(JSC::ExecState& state, const char* operationName)
    {
        JSC::VM& vm = state.vm();
        auto throwScope = DECLARE_THROW_SCOPE(vm);

        auto thisObject = jsEventTargetCast(vm, state.thisValue().toThis(&state, JSC::NotStrictMode));
        if (UNLIKELY(!thisObject))
            return throwThisTypeError(state, throwScope, "EventTarget", operationName);

        if (auto* window = thisObject->wrapped().toDOMWindow()) {
            if (!window->frame() || !BindingSecurity::shouldAllowAccessToDOMWindow(&state, *window, ThrowSecurityError))
                return JSC::JSValue::encode(JSC::jsUndefined());
        }

        return operation(&state, thisObject.get(), throwScope);
    }

};


} // namespace WebCore
