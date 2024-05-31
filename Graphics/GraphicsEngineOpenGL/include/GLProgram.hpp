/*
 *  Copyright 2024 Diligent Graphics LLC
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#pragma once

#include <vector>
#include <string>

#include "GLObjectWrapper.hpp"
#include "ShaderResourcesGL.hpp"
#include "PipelineResourceSignatureGLImpl.hpp"

namespace Diligent
{

class ShaderGLImpl;

class GLProgram
{
public:
    GLProgram(ShaderGLImpl* const* ppShaders,
              Uint32               NumShaders,
              bool                 IsSeparableProgram) noexcept;
    ~GLProgram();

    const GLObjectWrappers::GLProgramObj& GetGLHandle() const { return m_GLProg; }

    const std::string& GetInfoLog() const { return m_InfoLog; }

    enum class LinkStatus
    {
        Undefined,
        InProgress,
        Succeeded,
        Failed
    };
    LinkStatus GetLinkStatus(bool WaitForCompletion = false) noexcept;

    struct LoadResourcesAttribs
    {
        const SHADER_TYPE             ShaderStages;
        const PIPELINE_RESOURCE_FLAGS SamplerResourceFlag;
        class GLContextState&         State;
        const bool                    LoadUniformBufferReflection = false;
        const SHADER_SOURCE_LANGUAGE  SourceLang                  = SHADER_SOURCE_LANGUAGE_DEFAULT;
    };
    std::shared_ptr<const ShaderResourcesGL>& LoadResources(const LoadResourcesAttribs& Attribs);

    void ApplyBindings(const PipelineResourceSignatureGLImpl*            pSignature,
                       const ShaderResourcesGL&                          ProgResources,
                       GLContextState&                                   State,
                       const PipelineResourceSignatureGLImpl::TBindings& BaseBindings);


    std::shared_ptr<const ShaderResourcesGL>& GetResources()
    {
        return m_pResources;
    }

private:
    GLObjectWrappers::GLProgramObj m_GLProg{true};
    std::vector<ShaderGLImpl*>     m_AttachedShaders;
    std::string                    m_InfoLog;
    LinkStatus                     m_LinkStatus = LinkStatus::Undefined;

    std::shared_ptr<const ShaderResourcesGL> m_pResources;
};

} // namespace Diligent