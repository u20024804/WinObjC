//******************************************************************************
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//******************************************************************************

#pragma once

#include "ShaderInfo.h"

struct ShaderNode;
typedef map<string, ShaderNode*> ShaderDef;

class ShaderContext {
    ShaderLayout    shaderVars;

    ShaderDef       vs;
    ShaderDef       ps;

    string          temporaries;
    int             nextTemp;
    
protected:
    string generate(ShaderLayout& outputs, ShaderLayout& inputs, const ShaderDef& shader,
                    const string& desc);
    
public:
    ShaderContext(const ShaderDef& vert, const ShaderDef& pixel) :
        vs(vert), ps(pixel), nextTemp(0) {}

    string addTempExpr(string valExpr);
    
    GLKShaderPair* generate(ShaderLayout& inputs);
};

// --------------------------------------------------------------------------------

struct ShaderNode {
    virtual bool generate(string& out, ShaderContext& c, ShaderLayout& v) { return false; }
};

struct ShaderVarRef : public ShaderNode {    
    string name;
    string constantResult;
public:
    ShaderVarRef(const string& name, const string& constantResult = "") : name(name), constantResult(constantResult) {}

    virtual bool generate(string& out, ShaderContext& c, ShaderLayout& v) override;
};

struct ShaderFallbackRef : public ShaderNode {
    string first;
    string second;
    string constantResult;
public:
    ShaderFallbackRef(const string& first, const string& second, const string& constantResult = "") :
        first(first), second(second), constantResult(constantResult) {}

    virtual bool generate(string& out, ShaderContext& c, ShaderLayout& v) override;
};

struct ShaderPosRef : public ShaderNode {
public:
    inline ShaderPosRef() {}
    
    virtual bool generate(string& out, ShaderContext& c, ShaderLayout& v) override;
};

struct ShaderTexRef : public ShaderNode {
    string texVar;
    ShaderNode* uvRef;
    ShaderNode* nextRef;

public:
    ShaderTexRef(const string& tex, ShaderNode* uvRef, ShaderNode* nextRef) :
        texVar(rex), uvRef(uvRef), nextRef(nextRef) {}

    virtual bool generate(string& out, ShaderContext& c, ShaderLayout& v) override;
};
