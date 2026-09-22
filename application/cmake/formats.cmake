# MeshOp3D
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# ============================================================================
# MeshOp3D - Supported File Formats (Single Source of Truth)
# ============================================================================
#
# To ADD a new format:
#   1. Append its id to MOP_FORMATS.
#   2. Define the MOP_FORMAT_<id>_* variables for it.
#   That's it. All platform-specific association files are regenerated
#   automatically from this list:
#     - Linux:   share/applications/*.desktop  (MOP_DESKTOP_MIME_TYPES)
#     - Linux:   share/mime/packages/*.xml     (MOP_MIME_XML_ENTRIES)
#     - macOS:   Info.plist                    (MOP_PLIST_EXTENSIONS)  [Phase 2]
#     - Windows: NSIS installer                                         [Phase 3]
#
# Per-format fields (MOP_FORMAT_<id>_*):
#   EXT            - file extension without dot (lowercase)
#   MIME           - IANA MIME type string
#   DESC           - English human-readable description
#   DESC_IT        - Italian human-readable description (optional)
#   MAGIC_TYPE     - "string" or "little32" for magic byte detection (optional)
#   MAGIC_OFFSET   - byte offset for magic detection (optional)
#   MAGIC_VALUE    - magic value string; leave unset/empty to skip (optional)
#   MAGIC_PRIORITY - detection priority, higher wins (default: 50) (optional)
# ============================================================================

set(MOP_FORMATS ply off obj stl gltf glb)

# --- PLY (Polygon File Format / Stanford Triangle Format) ---
set(MOP_FORMAT_ply_EXT            "ply")
set(MOP_FORMAT_ply_MIME           "model/ply")
set(MOP_FORMAT_ply_DESC           "PLY 3D mesh")
set(MOP_FORMAT_ply_DESC_IT        "Mesh 3D PLY")
set(MOP_FORMAT_ply_MAGIC_TYPE     "string")
set(MOP_FORMAT_ply_MAGIC_OFFSET   "0")
set(MOP_FORMAT_ply_MAGIC_VALUE    "ply\\n")
set(MOP_FORMAT_ply_MAGIC_PRIORITY "50")

# --- OFF (Object File Format) ---
set(MOP_FORMAT_off_EXT     "off")
set(MOP_FORMAT_off_MIME    "application/x-off")
set(MOP_FORMAT_off_DESC    "OFF 3D mesh")
set(MOP_FORMAT_off_DESC_IT "Mesh 3D OFF")
# No reliable magic bytes for OFF

# --- OBJ (Wavefront Object) ---
set(MOP_FORMAT_obj_EXT     "obj")
set(MOP_FORMAT_obj_MIME    "application/x-wavefront-obj")
set(MOP_FORMAT_obj_DESC    "Wavefront OBJ 3D model")
set(MOP_FORMAT_obj_DESC_IT "Modello 3D Wavefront OBJ")
# No reliable magic bytes for OBJ

# --- STL (Stereolithography) ---
set(MOP_FORMAT_stl_EXT            "stl")
set(MOP_FORMAT_stl_MIME           "model/stl")
set(MOP_FORMAT_stl_DESC           "STL 3D mesh")
set(MOP_FORMAT_stl_DESC_IT        "Mesh 3D STL")
set(MOP_FORMAT_stl_MAGIC_TYPE     "string")
set(MOP_FORMAT_stl_MAGIC_OFFSET   "0")
set(MOP_FORMAT_stl_MAGIC_VALUE    "solid ")
set(MOP_FORMAT_stl_MAGIC_PRIORITY "50")

# --- glTF (GL Transmission Format - JSON) ---
set(MOP_FORMAT_gltf_EXT     "gltf")
set(MOP_FORMAT_gltf_MIME    "model/gltf+json")
set(MOP_FORMAT_gltf_DESC    "glTF 3D model")
set(MOP_FORMAT_gltf_DESC_IT "Modello 3D glTF")
# No reliable magic bytes for glTF JSON

# --- GLB (GL Transmission Format - Binary) ---
set(MOP_FORMAT_glb_EXT            "glb")
set(MOP_FORMAT_glb_MIME           "model/gltf-binary")
set(MOP_FORMAT_glb_DESC           "GLB 3D model (binary glTF)")
set(MOP_FORMAT_glb_DESC_IT        "Modello 3D GLB (glTF binario)")
set(MOP_FORMAT_glb_MAGIC_TYPE     "little32")
set(MOP_FORMAT_glb_MAGIC_OFFSET   "0")
set(MOP_FORMAT_glb_MAGIC_VALUE    "0x46546C67")  # 'glTF' little-endian
set(MOP_FORMAT_glb_MAGIC_PRIORITY "60")           # higher: binary detection is reliable

# ============================================================================
# Generated variables — do NOT edit below this line.
# All variables below are derived automatically from the definitions above.
# ============================================================================

# -- Linux: .desktop MimeType field -----------------------------------------
# Produces: "mime1;mime2;...;"
# Used as: MimeType=@MOP_DESKTOP_MIME_TYPES@ in application.desktop.in
set(MOP_DESKTOP_MIME_TYPES "")
foreach(_fmt IN LISTS MOP_FORMATS)
    string(APPEND MOP_DESKTOP_MIME_TYPES "${MOP_FORMAT_${_fmt}_MIME};")
endforeach()

# -- Linux: freedesktop MIME XML entries block --------------------------------
# Used as: @MOP_MIME_XML_ENTRIES@ in mop3d-mimetypes.xml (CMake template)
set(MOP_MIME_XML_ENTRIES "")
foreach(_fmt IN LISTS MOP_FORMATS)
    set(_ext     "${MOP_FORMAT_${_fmt}_EXT}")
    set(_mime    "${MOP_FORMAT_${_fmt}_MIME}")
    set(_desc    "${MOP_FORMAT_${_fmt}_DESC}")
    set(_desc_it "${MOP_FORMAT_${_fmt}_DESC_IT}")
    set(_mval    "${MOP_FORMAT_${_fmt}_MAGIC_VALUE}")

    string(APPEND MOP_MIME_XML_ENTRIES "  <!-- ${_desc} -->\n")
    string(APPEND MOP_MIME_XML_ENTRIES "  <mime-type type=\"${_mime}\">\n")
    string(APPEND MOP_MIME_XML_ENTRIES "    <comment>${_desc}</comment>\n")
    if(_desc_it)
        string(APPEND MOP_MIME_XML_ENTRIES "    <comment xml:lang=\"it\">${_desc_it}</comment>\n")
    endif()
    string(APPEND MOP_MIME_XML_ENTRIES "    <glob pattern=\"*.${_ext}\"/>\n")
    if(_mval)
        set(_mtype   "${MOP_FORMAT_${_fmt}_MAGIC_TYPE}")
        set(_moffset "${MOP_FORMAT_${_fmt}_MAGIC_OFFSET}")
        if(DEFINED MOP_FORMAT_${_fmt}_MAGIC_PRIORITY)
            set(_mprio "${MOP_FORMAT_${_fmt}_MAGIC_PRIORITY}")
        else()
            set(_mprio "50")
        endif()
        string(APPEND MOP_MIME_XML_ENTRIES "    <magic priority=\"${_mprio}\">\n")
        string(APPEND MOP_MIME_XML_ENTRIES "      <match type=\"${_mtype}\" offset=\"${_moffset}\" value=\"${_mval}\"/>\n")
        string(APPEND MOP_MIME_XML_ENTRIES "    </magic>\n")
    endif()
    string(APPEND MOP_MIME_XML_ENTRIES "  </mime-type>\n\n")
endforeach()

# -- macOS: Info.plist CFBundleTypeExtensions array entries (Phase 2) ---------
# Used as: @MOP_PLIST_EXTENSIONS@ inside the <array> in Info.plist.in
set(MOP_PLIST_EXTENSIONS "")
foreach(_fmt IN LISTS MOP_FORMATS)
    string(APPEND MOP_PLIST_EXTENSIONS "        <string>${MOP_FORMAT_${_fmt}_EXT}</string>\n")
endforeach()

# -- Windows: NSIS installer (Phase 3) ----------------------------------------
# Registry entries will be generated in Phase 3 using MOP_FORMATS and
# MOP_FORMAT_<id>_* variables defined above.
