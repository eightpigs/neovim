#ifndef NVIM_EXTMARK_DEFS_H
#define NVIM_EXTMARK_DEFS_H

#include "nvim/types.h"
#include "nvim/lib/kvec.h"

typedef struct {
  char *text;
  int hl_id;
} VirtTextChunk;

typedef kvec_t(VirtTextChunk) VirtText;
#define VIRTTEXT_EMPTY ((VirtText)KV_INITIAL_VALUE)

typedef struct
{
  int hl_id;  // highlight group
  VirtText virt_text;
  // TODO(bfredl): style, signs, etc
  bool shared;  // shared decoration, don't free
} Decoration;

typedef struct
{
  uint64_t ns_id;
  uint64_t mark_id;
  // TODO(bfredl): a lot of small allocations. Should probably use
  // kvec_t(Decoration) as an arena. Alternatively, store ns_id/mark_id
  // _inline_ in MarkTree and use the map only for decorations.
  Decoration *decor;
} ExtmarkItem;

typedef struct undo_object ExtmarkUndoObject;
typedef kvec_t(ExtmarkUndoObject) extmark_undo_vec_t;

// Undo/redo extmarks

typedef enum {
  kExtmarkNOOP,        // Extmarks shouldn't be moved
  kExtmarkUndo,        // Operation should be reversable/undoable
  kExtmarkNoUndo,      // Operation should not be reversable
  kExtmarkUndoNoRedo,  // Operation should be undoable, but not redoable
} ExtmarkOp;

typedef struct {
  NS ns_id;
  bool active;
  LuaRef redraw_start;
  LuaRef redraw_buf;
  LuaRef redraw_win;
  LuaRef redraw_line;
  LuaRef redraw_end;
} DecorationProvider;

#define DECORATION_PROVIDER_INIT(ns_id) (DecorationProvider) \
                                 { ns_id, false, LUA_NOREF, LUA_NOREF, \
                                   LUA_NOREF, LUA_NOREF, LUA_NOREF }

#endif  // NVIM_EXTMARK_DEFS_H
