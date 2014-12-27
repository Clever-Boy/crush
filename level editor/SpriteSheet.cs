﻿/*********************************************************************
Matt Marchant 2014
http://trederia.blogspot.com

Crush Map Editor - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

using Newtonsoft.Json;

namespace Level_editor
{
    struct FloatRect
    {
        public float x, y, w, h;
    }
    
    struct Frame
    {
        public FloatRect frame;
        public bool rotated;
        public bool trimmed;
        public FloatRect spriteSourceSize;
        public SizeF pivot;
    }

    class SpriteSheet
    {
        private List<Frame> m_frames;
        public List<Frame> Frames
        {
            get { return m_frames; }
        }

        public SpriteSheet()
        {
            m_frames = new List<Frame>();
        }

        public static void LoadFile(string path, SpriteSheet sheet)
        {
            JsonSerializer js = new JsonSerializer();
            js.NullValueHandling = NullValueHandling.Ignore;

            using (StreamReader sr = new StreamReader(path))
            using (JsonReader jr = new JsonTextReader(sr))
            {
                sheet = js.Deserialize<SpriteSheet>(jr);
            }
        }
    }
}