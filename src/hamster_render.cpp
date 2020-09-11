static RenderQueue
render_create_queue(u32 size)
{
    RenderQueue queue = {};
    
    queue.max_size = size;
    queue.entries = malloc(queue.max_size);
    
    return queue;
}

static void 
render_destory_queue(RenderQueue *queue)
{
    free(queue->entries);
}

static void
*_render_push_entry(RenderQueue *queue, u32 struct_size, RenderHeader type)
{
    void *result = 0;
    
    assert(queue->max_size > queue->size + struct_size);
    
    result = (void *)((u8 *)queue->entries + queue->size);
    queue->size += struct_size;
    queue->len += 1;
    
    *(RenderHeader *)result = type;
    
    return result;
}

static void 
render_push_skybox(RenderQueue *queue, Cubemap skybox)
{
    RenderEntrySkybox *entry = render_push_entry(queue, RenderEntrySkybox);
    
    entry->cube = skybox;
}

static void
render_push_line(RenderQueue *queue, Line line)
{
    RenderEntryLine *entry = render_push_entry(queue, RenderEntryLine);
    
    entry->line = line;
}

static void
render_push_hitbox(RenderQueue *queue, Entity entity)
{
    RenderEntryHitbox *entry = render_push_entry(queue, RenderEntryHitbox);
    
    entry->position = entity.position;
    entry->size = entity.size;
    entry->orientation = entity.rotate;
    entry->hbox = entity.model->hitboxes;
    entry->hbox_len = entity.model->hitboxes_len;
}

static void
render_push_ui(RenderQueue *queue, UIElement element)
{
    RenderEntryUI *entry = render_push_entry(queue, RenderEntryUI);
    
    entry->element = element;
}

static void
render_push_model_newest(RenderQueue *queue, Entity entity)
{
    RenderEntryModelNewest *entry = render_push_entry(queue, RenderEntryModelNewest);
    
    entry->position = entity.position;
    entry->size = entity.size;
    entry->orientation = entity.rotate;
    entry->model = entity.model;
}

static void
render_push_model(RenderQueue *queue, Entity entity)
{
    RenderEntryModel *entry = render_push_entry(queue, RenderEntryModel);
    
    entry->position = entity.position;
    entry->size = entity.size;
    entry->orientation = entity.rotate;
    entry->model = entity.model;
}

static void
render_load_uniforms(RenderContext *ctx, i32 index)
{
    GLuint pid = ctx->programs[index].id;
    
    ctx->program_uniforms[index].model = glGetUniformLocation(pid, "model");
    ctx->program_uniforms[index].view = glGetUniformLocation(pid, "view");
    ctx->program_uniforms[index].proj = glGetUniformLocation(pid, "proj");
    ctx->program_uniforms[index].transform = glGetUniformLocation(pid, "transform");
    ctx->program_uniforms[index].ortho = glGetUniformLocation(pid, "ortho");
    ctx->program_uniforms[index].tex_sampler = glGetUniformLocation(pid, "tex_sampler");
    ctx->program_uniforms[index].view_pos = glGetUniformLocation(pid, "view_pos");
    ctx->program_uniforms[index].light_pos = glGetUniformLocation(pid, "light_pos");
    ctx->program_uniforms[index].spotlight_direction = glGetUniformLocation(pid, "spotlight.direction");
    ctx->program_uniforms[index].spotlight_cutoff = glGetUniformLocation(pid, "spotlight.cutoff");
    ctx->program_uniforms[index].spotlight_outer_cutoff = glGetUniformLocation(pid, "spotlight.outer_cutoff");
    ctx->program_uniforms[index].spotlight_ambient_component = glGetUniformLocation(pid, "spotlight.ambient_component");
    ctx->program_uniforms[index].spotlight_diffuse_component = glGetUniformLocation(pid, "spotlight.diffuse_component");
    ctx->program_uniforms[index].spotlight_specular_component = glGetUniformLocation(pid, "spotlight.specular_component");
    ctx->program_uniforms[index].spotlight_atten_const = glGetUniformLocation(pid, "spotlight.atten_const");
    ctx->program_uniforms[index].spotlight_atten_linear = glGetUniformLocation(pid, "spotlight.atten_linear");
    ctx->program_uniforms[index].spotlight_atten_quad = glGetUniformLocation(pid, "spotlight.atten_quad");
    ctx->program_uniforms[index].direct_light_direction = glGetUniformLocation(pid, "direct_light.direction");
    ctx->program_uniforms[index].direct_light_ambient_component = glGetUniformLocation(pid, "direct_light.ambient_component");
    ctx->program_uniforms[index].direct_light_diffuse_component = glGetUniformLocation(pid, "direct_light.diffuse_component");
    ctx->program_uniforms[index].direct_light_specular_component = glGetUniformLocation(pid, "direct_light.specular_component");
    ctx->program_uniforms[index].point_light_position = glGetUniformLocation(pid, "point_light.position");
    ctx->program_uniforms[index].point_light_ambient_part = glGetUniformLocation(pid, "point_light.ambient_part");
    ctx->program_uniforms[index].point_light_diffuse_part = glGetUniformLocation(pid, "point_light.diffuse_part");
    ctx->program_uniforms[index].point_light_specular_part = glGetUniformLocation(pid, "point_light.specular_part");
    ctx->program_uniforms[index].point_light_atten_const = glGetUniformLocation(pid, "point_light.atten_const");
    ctx->program_uniforms[index].point_light_atten_linear = glGetUniformLocation(pid, "point_light.atten_linear");
    ctx->program_uniforms[index].point_light_atten_quad = glGetUniformLocation(pid, "point_light.atten_quad");
    ctx->program_uniforms[index].show_normal_map = glGetUniformLocation(pid, "show_normal_map");
    ctx->program_uniforms[index].use_mapped_normals = glGetUniformLocation(pid, "use_mapped_normals");
    ctx->program_uniforms[index].material_ambient_component = glGetUniformLocation(pid, "material.ambient_component");
    ctx->program_uniforms[index].material_diffuse_component = glGetUniformLocation(pid, "material.diffuse_component");
    ctx->program_uniforms[index].material_specular_component = glGetUniformLocation(pid, "material.specular_component");
    ctx->program_uniforms[index].material_specular_exponent = glGetUniformLocation(pid, "material.specular_exponent");
}

static void
render_load_programs(RenderContext *ctx)
{
    ctx->programs[ShaderProgram_Basic] = program_create_from_file(MAIN_VERTEX_FILENAME, MAIN_FRAG_FILENAME);
    ctx->programs[ShaderProgram_Simple] = program_create_from_file(SIMPLE_VERTEX_FILENAME, SIMPLE_FRAG_FILENAME);
    ctx->programs[ShaderProgram_Skybox] = program_create_from_file(SKYBOX_VERTEX_FILENAME, SKYBOX_FRAG_FILENAME);
    ctx->programs[ShaderProgram_UI] = program_create_from_file(UI_VERTEX_FILENAME, UI_FRAG_FILENAME);
    ctx->programs[ShaderProgram_Line] = program_create_from_file(MAIN_VERTEX_FILENAME, LINE_FRAG_FILENAME);
    ctx->programs[ShaderProgram_HDR] = 
        program_create_from_file(HDR_VERTEX_FILENAME, HDR_FRAG_FILENAME);
    
    for(u32 i = 0; i < (u32)ShaderProgram_LastElement; i++)
    {
        render_load_uniforms(ctx, i);
    }
}

static void
render_prepass(RenderContext *ctx)
{
    for(u32 i = 0; i < (u32)ShaderProgram_LastElement; i++)
    {
        ShaderProgram *prog = &ctx->programs[i];
        
        time_t vstamp = get_file_stamp(prog->vertex_filename);
        time_t fstamp = get_file_stamp(prog->fragment_filename);
        
        if(vstamp > prog->vertex_stamp || fstamp > prog->fragment_stamp)
        {
            ShaderProgram new_program = program_create_from_file(prog->vertex_filename, prog->fragment_filename);
            
            if(new_program.id != 0)
            {
                glDeleteProgram(prog->id);
                
                printf("Reloaded program from [%s] and [%s]\n", prog->vertex_filename, prog->fragment_filename);
                *prog = new_program;
                render_load_uniforms(ctx, i);
            }
            else
            {
                prog->vertex_stamp = vstamp;
                prog->fragment_stamp = fstamp;
            }
        }
    }
}

static void
get_frustum_planes(RenderContext *ctx)
{
    Mat4 vp = mul(ctx->proj, ctx->lookat);
    
    ctx->frustum_planes[FrustumPlane_Left].normal.x = vp.a[0][3] + vp.a[0][0];
    ctx->frustum_planes[FrustumPlane_Left].normal.y = vp.a[1][3] + vp.a[1][0];
    ctx->frustum_planes[FrustumPlane_Left].normal.z = vp.a[2][3] + vp.a[2][0];
    ctx->frustum_planes[FrustumPlane_Left].d = vp.a[3][3] + vp.a[3][0];
    
    ctx->frustum_planes[FrustumPlane_Right].normal.x = vp.a[0][3] - vp.a[0][0];
    ctx->frustum_planes[FrustumPlane_Right].normal.y = vp.a[1][3] - vp.a[1][0];
    ctx->frustum_planes[FrustumPlane_Right].normal.z = vp.a[2][3] - vp.a[2][0];
    ctx->frustum_planes[FrustumPlane_Right].d = vp.a[3][3] - vp.a[3][0];
    
    ctx->frustum_planes[FrustumPlane_Bottom].normal.x = vp.a[0][3] + vp.a[0][1];
    ctx->frustum_planes[FrustumPlane_Bottom].normal.y = vp.a[1][3] + vp.a[1][1];
    ctx->frustum_planes[FrustumPlane_Bottom].normal.z = vp.a[2][3] + vp.a[2][1];
    ctx->frustum_planes[FrustumPlane_Bottom].d = vp.a[3][3] + vp.a[3][1];
    
    ctx->frustum_planes[FrustumPlane_Top].normal.x = vp.a[0][3] - vp.a[0][1];
    ctx->frustum_planes[FrustumPlane_Top].normal.y = vp.a[1][3] - vp.a[1][1];
    ctx->frustum_planes[FrustumPlane_Top].normal.z = vp.a[2][3] - vp.a[2][1];
    ctx->frustum_planes[FrustumPlane_Top].d = vp.a[3][3] - vp.a[3][1];
    
    ctx->frustum_planes[FrustumPlane_Near].normal.x = vp.a[0][3] + vp.a[0][2];
    ctx->frustum_planes[FrustumPlane_Near].normal.y = vp.a[1][3] + vp.a[1][2];
    ctx->frustum_planes[FrustumPlane_Near].normal.z = vp.a[2][3] + vp.a[2][2];
    ctx->frustum_planes[FrustumPlane_Near].d = vp.a[3][3] + vp.a[3][2];
    
    ctx->frustum_planes[FrustumPlane_Far].normal.x = vp.a[0][3] - vp.a[0][2];
    ctx->frustum_planes[FrustumPlane_Far].normal.y = vp.a[1][3] - vp.a[1][2];
    ctx->frustum_planes[FrustumPlane_Far].normal.z = vp.a[2][3] - vp.a[2][2];
    ctx->frustum_planes[FrustumPlane_Far].d = vp.a[3][3] - vp.a[3][2];
    
    for(u32 i = 0; i < FrustumPlane_ElementCount; i++)
    {
        f32 nozf = inverse_len(ctx->frustum_planes[i].normal);
        ctx->frustum_planes[i].normal = scale(ctx->frustum_planes[i].normal, nozf);
        ctx->frustum_planes[i].d *= nozf;
    }
}

static void 
render_flush(RenderQueue *queue, RenderContext *ctx)
{
    render_prepass(ctx);
    get_frustum_planes(ctx);
    
    glBindFramebuffer(GL_FRAMEBUFFER, ctx->hdr_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderHeader *header = (RenderHeader *)queue->entries;
    int drawn = 0;
    
    for(u32 i = 0; i < queue->len; i++)
    {
        switch(*header)
        {
            case RenderType_RenderEntrySkybox:
            {
                GLuint program_id = ctx->programs[ShaderProgram_Skybox].id;
                auto uniloc = &ctx->program_uniforms[ShaderProgram_Skybox];
                
                glUseProgram(program_id);
                Mat4 view_no_translation = ctx->lookat;
                view_no_translation.columns[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
                opengl_set_uniform(uniloc->view, view_no_translation);
                opengl_set_uniform(uniloc->proj, ctx->proj);
                
                RenderEntrySkybox *entry = (RenderEntrySkybox *)header;
                glBindVertexArray(entry->cube.vao);
                glBindTexture(GL_TEXTURE_CUBE_MAP, entry->cube.texture);
                
                glDepthFunc(GL_LEQUAL);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
                glDepthFunc(GL_LESS);
                
                header = (RenderHeader *)(++entry);
            } break;
            case RenderType_RenderEntryLine:
            {
                // TODO(mateusz): Here and in the RenderEntryHitbox we are creating
                // and destroying a new vao for each draw call, that's not really optimal.
                // We maybe could use some transient gpu memory that's passed around with
                // the RenderContext??
                GLuint program_id = ctx->programs[ShaderProgram_Line].id;
                auto uniloc = &ctx->program_uniforms[ShaderProgram_Line];
                glUseProgram(program_id);
                
                GLuint vao = 0;
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);
                
                GLuint vbo = 0;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                
                RenderEntryLine *entry = (RenderEntryLine *)header;
                glBufferData(GL_ARRAY_BUFFER, sizeof(entry->line), &entry->line, GL_STATIC_DRAW);
                
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), nullptr);
                glEnableVertexAttribArray(0);
                
                opengl_set_uniform(uniloc->view, ctx->lookat);
                opengl_set_uniform(uniloc->proj, ctx->proj);
                // TODO(mateusz): Write a diffrent line shader so we don't have to
                // cover ourselvs with setting the model to identify.
                opengl_set_uniform(uniloc->model, Mat4(1.0f));
                
                glDrawArrays(GL_LINES, 0, 2);
                
                glDeleteVertexArrays(1, &vao);
                glDeleteBuffers(1, &vbo);
                
                header = (RenderHeader *)(++entry);
            }break;
            case RenderType_RenderEntryHitbox:
            {
                GLuint program_id = ctx->programs[ShaderProgram_Line].id;
                auto uniloc = &ctx->program_uniforms[ShaderProgram_Line];
                glUseProgram(program_id);
                
                opengl_set_uniform(uniloc->view, ctx->lookat);
                opengl_set_uniform(uniloc->proj, ctx->proj);
                
                RenderEntryHitbox *entry = (RenderEntryHitbox *)header;
                for(u32 i = 0; i < entry->hbox_len; i++)
                {
                    Hitbox *hbox = entry->hbox + i;
                    Vec3 vertices[VERTICES_PER_CUBE] = {
                        hbox->refpoint,
                        add(hbox->refpoint, Vec3(hbox->size.x, 0.0f, 0.0f)), // x
                        add(hbox->refpoint, Vec3(0.0f, hbox->size.y, 0.0f)), // y
                        add(hbox->refpoint, Vec3(0.0f, 0.0f, hbox->size.z)), // z
                        add(hbox->refpoint, Vec3(hbox->size.x, hbox->size.y, 0.0f)), // xy
                        add(hbox->refpoint, Vec3(0.0f, hbox->size.y, hbox->size.z)), // yz
                        add(hbox->refpoint, Vec3(hbox->size.x, 0.0f, hbox->size.z)), // xz
                        add(hbox->refpoint, hbox->size),
                    };
                    
                    u32 indicies[INDICES_PER_CUBE] = {
                        0, 1, // start to x
                        1, 4, // x to xy
                        4, 2, // xy to y
                        2, 0, // y to start
                        
                        0, 3, // start to z
                        1, 6, // x to xz
                        4, 7, // xy to xyz
                        2, 5, // y to yz
                        
                        3, 6, // z to xz
                        6, 7, // xz to xyz
                        7, 5, // xyz to yz
                        5, 3, // yz to 
                    };
                    
                    GLuint hbox_vao, hbox_vbo, hbox_ebo = 0;
                    
                    glGenVertexArrays(1, &hbox_vao);
                    glGenBuffers(1, &hbox_vbo);
                    glGenBuffers(1, &hbox_ebo);
                    
                    glBindVertexArray(hbox_vao);
                    glBindBuffer(GL_ARRAY_BUFFER, hbox_vbo);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hbox_ebo);
                    
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), nullptr);
                    glEnableVertexAttribArray(0);
                    
                    Mat4 transform = scale(Mat4(1.0f), entry->size);
                    transform = rotate_quat(transform, entry->orientation);
                    transform = translate(transform, entry->position);
                    opengl_set_uniform(uniloc->model, transform);
                    
                    glDrawElements(GL_LINES, ARRAY_LEN(indicies), GL_UNSIGNED_INT, NULL);
                    
                    glDeleteVertexArrays(1, &hbox_vao);
                    glDeleteBuffers(1, &hbox_vbo);
                    glDeleteBuffers(1, &hbox_ebo);
                }
                
                header = (RenderHeader *)(++entry);
            }break;
            case RenderType_RenderEntryUI:
            {
                GLuint program_id = ctx->programs[ShaderProgram_UI].id;
                auto uniloc = &ctx->program_uniforms[ShaderProgram_UI];
                glUseProgram(program_id);
                
                RenderEntryUI *entry = (RenderEntryUI *)header;
                
                Vec3 pos = Vec3(entry->element.position.x, entry->element.position.y, 0.0f);
                Vec3 size = Vec3(entry->element.size.x, entry->element.size.y, 1.0f);
                
                Mat4 transform = translate(Mat4(1.0f), pos);
                transform = scale(transform, size);
                
                opengl_set_uniform(uniloc->transform, transform);
                opengl_set_uniform(uniloc->ortho, ctx->ortho);
                
                glEnable(GL_BLEND);
                
                glBindVertexArray(entry->element.vao);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, entry->element.texture);
                opengl_set_uniform(uniloc->tex_sampler, 0);
                
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                
                glDisable(GL_BLEND);
                // NOTE(mateusz): I don't know why this was here, but i guess I did a hack for it.
                // I broke the rendering of UI elements on top of the skybox, now it works.
                //glDisable(GL_DEPTH_TEST);
                //glEnable(GL_DEPTH_TEST);
                
                header = (RenderHeader *)(++entry);
            }break;
            case RenderType_RenderEntryModelNewest:
            {
                GLuint program_id = ctx->programs[ShaderProgram_Basic].id;
                auto uniloc = &ctx->program_uniforms[ShaderProgram_Basic];
                glUseProgram(program_id);
                
                opengl_set_uniform(uniloc->view, ctx->lookat);
                opengl_set_uniform(uniloc->proj, ctx->proj);
                
                opengl_set_uniform(uniloc->view_pos, ctx->cam.position);
                
                opengl_set_uniform(uniloc->light_pos, ctx->spot.position);
                opengl_set_uniform(uniloc->spotlight_direction, ctx->spot.direction);
                opengl_set_uniform(uniloc->spotlight_cutoff, cosf(to_radians(ctx->spot.cutoff)));
                opengl_set_uniform(uniloc->spotlight_outer_cutoff, cosf(to_radians(ctx->spot.outer_cutoff)));
                opengl_set_uniform(uniloc->spotlight_ambient_component, ctx->spot.ambient_part);
                opengl_set_uniform(uniloc->spotlight_diffuse_component, ctx->spot.diffuse_part);
                opengl_set_uniform(uniloc->spotlight_specular_component, ctx->spot.specular_part);
                opengl_set_uniform(uniloc->spotlight_atten_const, ctx->spot.atten_const);
                opengl_set_uniform(uniloc->spotlight_atten_linear, ctx->spot.atten_linear);
                opengl_set_uniform(uniloc->spotlight_atten_quad, ctx->spot.atten_quad);
                
                opengl_set_uniform(uniloc->direct_light_direction, ctx->sun.direction);
                opengl_set_uniform(uniloc->direct_light_ambient_component, ctx->sun.ambient_part);
                opengl_set_uniform(uniloc->direct_light_diffuse_component, ctx->sun.diffuse_part);
                opengl_set_uniform(uniloc->direct_light_specular_component, ctx->sun.specular_part);
                
                opengl_set_uniform(uniloc->point_light_position, ctx->point_light.position);
                
                opengl_set_uniform(uniloc->point_light_ambient_part, ctx->point_light.ambient_part);
                opengl_set_uniform(uniloc->point_light_diffuse_part, ctx->point_light.diffuse_part);
                opengl_set_uniform(uniloc->point_light_specular_part, ctx->point_light.specular_part);
                
                opengl_set_uniform(uniloc->point_light_atten_const, ctx->point_light.atten_const);
                opengl_set_uniform(uniloc->point_light_atten_linear, ctx->point_light.atten_linear);
                opengl_set_uniform(uniloc->point_light_atten_quad, ctx->point_light.atten_quad);
                
                opengl_set_uniform(uniloc->show_normal_map, ctx->show_normal_map);
                opengl_set_uniform(uniloc->use_mapped_normals, ctx->use_mapped_normals);
                
                RenderEntryModelNewest *entry = (RenderEntryModelNewest *)header;
                
                Mat4 transform = scale(Mat4(1.0f), entry->size);
                transform = rotate_quat(transform, entry->orientation);
                transform = translate(transform, entry->position);
                opengl_set_uniform(uniloc->model, transform);
                
                Model *model = entry->model;
                for(u32 i = 0; i < model->meshes_len; i++)
                {
                    if(!hitbox_in_frustum(model->hitboxes + i, ctx->frustum_planes, transform))
                    {
                        continue;
                    }
                    glBindVertexArray(model->meshes[i].vao);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, model->texture);
                    drawn++;
                    Mesh *mesh = &model->meshes[i];
                    Material *material = NULL;
                    for(u32 i = 0; i < model->materials_len; i++)
                    {
                        if(strings_match(mesh->material_name, model->materials[i].name))
                        {
                            material = &model->materials[i];
                            break;
                        }
                    }
                    
                    if(material)
                    {
                        if(material->flags & MATERIAL_FLAGS_HAS_DIFFUSE_MAP)
                        {
                            glUniform1i(glGetUniformLocation(program_id, "diffuse_map"), 1);
                            glActiveTexture(GL_TEXTURE1);
                            glBindTexture(GL_TEXTURE_2D, material->diffuse_map);
                        }
                        if(material->flags & MATERIAL_FLAGS_HAS_SPECULAR_MAP)
                        {
                            glUniform1i(glGetUniformLocation(program_id, "specular_map"), 2);
                            glActiveTexture(GL_TEXTURE2);
                            glBindTexture(GL_TEXTURE_2D, material->specular_map);
                        }
                        if(material->flags & MATERIAL_FLAGS_HAS_NORMAL_MAP)
                        {
                            glUniform1i(glGetUniformLocation(program_id, "normal_map"), 3);
                            glActiveTexture(GL_TEXTURE3);
                            glBindTexture(GL_TEXTURE_2D, material->normal_map);
                        }
                        
                        opengl_set_uniform(uniloc->material_ambient_component, material->ambient_component);
                        opengl_set_uniform(uniloc->material_diffuse_component, material->diffuse_component);
                        opengl_set_uniform(uniloc->material_specular_component, material->specular_component);
                        opengl_set_uniform(uniloc->material_specular_exponent, material->specular_exponent);
                    }
                    else
                    {
                        Vec3 one = Vec3(1.0f, 1.0f, 1.0f);
                        opengl_set_uniform(uniloc->material_ambient_component, one);
                        opengl_set_uniform(uniloc->material_diffuse_component, one);
                        opengl_set_uniform(uniloc->material_specular_component, one);
                        opengl_set_uniform(uniloc->material_specular_exponent, 1.0f);
                    }
                    
                    glDrawElements(GL_TRIANGLES, mesh->indices_len, GL_UNSIGNED_INT, NULL);
                }
                
                header = (RenderHeader *)(++entry);
            }break;
            case RenderType_RenderEntryModel:
            {
                GLuint program_id = ctx->programs[ShaderProgram_Simple].id;
                glUseProgram(program_id);
                
                opengl_set_uniform(program_id, "view", ctx->lookat);
                opengl_set_uniform(program_id, "proj", ctx->proj);
                
                opengl_set_uniform(program_id, "view_pos", ctx->cam.position);
                
                opengl_set_uniform(program_id, "light_pos", ctx->spot.position);
                opengl_set_uniform(program_id, "spotlight.direction", ctx->spot.direction);
                opengl_set_uniform(program_id, "spotlight.cutoff", cosf(to_radians(ctx->spot.cutoff)));
                opengl_set_uniform(program_id, "spotlight.outer_cutoff", cosf(to_radians(ctx->spot.outer_cutoff)));
                opengl_set_uniform(program_id, "spotlight.ambient_component", ctx->spot.ambient_part);
                opengl_set_uniform(program_id, "spotlight.diffuse_component", ctx->spot.diffuse_part);
                opengl_set_uniform(program_id, "spotlight.specular_component", ctx->spot.specular_part);
                opengl_set_uniform(program_id, "spotlight.atten_const", ctx->spot.atten_const);
                opengl_set_uniform(program_id, "spotlight.atten_linear", ctx->spot.atten_linear);
                opengl_set_uniform(program_id, "spotlight.atten_quad", ctx->spot.atten_quad);
                
                opengl_set_uniform(program_id, "direct_light.direction", Vec3(0.0f, -1.0f, 0.0f));
                opengl_set_uniform(program_id, "direct_light.ambient_component", Vec3(0.05f, 0.05f, 0.05f));
                opengl_set_uniform(program_id, "direct_light.diffuse_component", Vec3(0.2f, 0.2f, 0.2f));
                opengl_set_uniform(program_id, "direct_light.specular_component", Vec3(0.4f, 0.4f, 0.4f));
                
                opengl_set_uniform(program_id, "show_normal_map", ctx->show_normal_map);
                opengl_set_uniform(program_id, "use_mapped_normals", ctx->use_mapped_normals);
                
                RenderEntryModel *entry = (RenderEntryModel *)header;
                
                Mat4 transform = scale(Mat4(1.0f), entry->size);
                transform = rotate_quat(transform, entry->orientation);
                transform = translate(transform, entry->position);
                opengl_set_uniform(program_id, "model", transform);
                
                Model *model = entry->model;
                for(u32 i = 0; i < model->meshes_len; i++)
                {
                    glBindVertexArray(model->meshes[i].vao);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, model->texture);
                    
                    Mesh *mesh = &model->meshes[i];
                    Material *material = NULL;
                    for(u32 i = 0; i < model->materials_len; i++)
                    {
                        if(strings_match(mesh->material_name, model->materials[i].name))
                        {
                            material = &model->materials[i];
                            break;
                        }
                    }
                    
                    if(material)
                    {
                        opengl_set_uniform(program_id, "material.ambient_component", material->ambient_component);
                        opengl_set_uniform(program_id, "material.diffuse_component", material->diffuse_component);
                        opengl_set_uniform(program_id, "material.specular_component", material->specular_component);
                        opengl_set_uniform(program_id, "material.specular_exponent", material->specular_exponent);
                    }
                    else
                    {
                        Vec3 one = Vec3(1.0f, 1.0f, 1.0f);
                        opengl_set_uniform(program_id, "material.ambient_component", one);
                        opengl_set_uniform(program_id, "material.diffuse_component", one);
                        opengl_set_uniform(program_id, "material.specular_component", one);
                        opengl_set_uniform(program_id, "material.specular_exponent", 1.0f);
                    }
                    
                    glDrawElements(GL_TRIANGLES, mesh->indices_len, GL_UNSIGNED_INT, NULL);
                }
                
                header = (RenderHeader *)(++entry);
            }break;
        }
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // TODO: Each element stores the same infomation in it's own vao and vbo,
    // it would be faster and cheaper if each of them used the same one.
    f32 vertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    GLuint vao = 0;
    GLuint vbo = 0;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(ctx->programs[ShaderProgram_HDR].id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctx->color_buffer);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
    //printf("drawn = %d\n", drawn);
    queue->len = 0;
    queue->size = 0;
    memset(queue->entries, 0, queue->max_size);
}

// Takes a compiled shader, checks if it produced an error
// returns false if it did, true otherwise.
static bool
program_shader_ok(GLuint shader)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("%s\n", infoLog);
        return false;
    }
    
    return true;
}

// Takes a linked program, checks if it produced an error
// returns false if it did, true otherwise.
static bool
program_ok(GLuint program)
{
    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("%s\n", infoLog);
        return false;
    }
    
    return true;
}

static ShaderProgram
program_create_from_file(const char *vertex_filename, const char *fragment_filename)
{
    ShaderProgram program = {};
    program.vertex_filename = vertex_filename;
    program.fragment_filename = fragment_filename;
    
    program.vertex_stamp = get_file_stamp(vertex_filename);
    program.fragment_stamp = get_file_stamp(fragment_filename);
    
    FILE *f = fopen(vertex_filename, "r");
    assert(f);
    char *vertex_src = read_file_to_string(f);
    fclose(f);
    
    f = fopen(fragment_filename, "r");
    assert(f);
    char *fragment_src = read_file_to_string(f);
    fclose(f);
    
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_src, NULL);
    glCompileShader(vertex_shader);
    if(!program_shader_ok(vertex_shader))
    {
        printf("Error in file [%s]\n", vertex_filename);
        program.id = 0;
        return program;
    }
    
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_src, NULL);
    glCompileShader(fragment_shader);
    if(!program_shader_ok(fragment_shader))
    {
        printf("Error in file [%s]\n", fragment_filename);
        program.id = 0;
        return program;
    }
    
    program.id = glCreateProgram();
    glAttachShader(program.id, vertex_shader);
    glAttachShader(program.id, fragment_shader);
    glLinkProgram(program.id);
    assert(program_ok(program.id));
    
    free(vertex_src);
    free(fragment_src);
    
    return program;
}

static void
camera_calculate_vectors(Camera *cam)
{
    const Vec3 world_up(0.0f, 1.0f, 0.0f);
    cam->right = noz(cross(cam->front, world_up));
    cam->up = noz(cross(cam->right, cam->front));
}

static void
camera_mouse_moved(Camera *cam, f32 dx, f32 dy)
{
    f32 sensitivity = 0.2f; // TODO: move it outside
    
    cam->yaw += to_radians(dx * sensitivity);
    cam->pitch += to_radians(-dy * sensitivity);
    
    if(cam->pitch > to_radians(89.0f)) {
        cam->pitch = to_radians(89.0f);
    } else if(cam->pitch < to_radians(-89.0f)) {
        cam->pitch = to_radians(-89.0f);
    }
    
    cam->front.x = cosf(cam->pitch) * cosf(cam->yaw);
    cam->front.y = sinf(cam->pitch);
    cam->front.z = cosf(cam->pitch) * sinf(cam->yaw);
    cam->front = noz(cam->front);
    camera_calculate_vectors(cam);
}

static void
opengl_set_uniform(GLuint location, i32 v)
{
    glUniform1i(location, v);
    assert(glGetError() == GL_NO_ERROR);
}

static void
opengl_set_uniform(GLuint program, const char *name, i32 v)
{
    GLint location = glGetUniformLocation(program, name);
    opengl_set_uniform(location, v);
}

static void 
opengl_set_uniform(GLuint location, f32 val)
{
    glUniform1f(location, val);
    assert(glGetError() == GL_NO_ERROR);
}

static void 
opengl_set_uniform(GLuint program, const char *name, f32 val)
{
    GLint location = glGetUniformLocation(program, name);
    opengl_set_uniform(location, val);
}

static void
opengl_set_uniform(GLuint location, Vec3 vec)
{
    glUniform3fv(location, 1, vec.m);
    assert(glGetError() == GL_NO_ERROR);
}

static void
opengl_set_uniform(GLuint program, const char *name, Vec3 vec)
{
    GLint location = glGetUniformLocation(program, name);
    opengl_set_uniform(location, vec);
}

static void
opengl_set_uniform(GLuint location, Mat4 mat, GLboolean transpose)
{
    glUniformMatrix4fv(location, 1, transpose, mat.a1d);
    assert(glGetError() == GL_NO_ERROR);
}

static void
opengl_set_uniform(GLuint program, const char *name, Mat4 mat, GLboolean transpose)
{
    GLint location = glGetUniformLocation(program, name);
    opengl_set_uniform(location, mat, transpose);
}