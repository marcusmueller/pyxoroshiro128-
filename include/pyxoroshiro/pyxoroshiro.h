#ifndef PYXOROSHIRO_H
#define PYXOROSHIRO_H
#include <Python.h>
#include "structmember.h"
PyObject* pyxoroshiro_rand(PyObject *self, PyObject *args);
PyMODINIT_FUNC PyInit_pyxoroshiro(void);

PyObject *PyxoroshiroError;

PyObject *PyxoroshiroRng;

typedef struct {
  PyObject_HEAD
  uint64_t state[2];
  /* specific fields */
} rng;
static void pyxoroshiro_dealloc(rng *self) {
  Py_TYPE(self)->tp_free((PyObject*)self);
}
PyObject *rng_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
int rng_init(rng *self, PyObject *args, PyObject *kwds);

PyObject *rng_rand(rng *self, PyObject *args, PyObject *kwds);
PyObject *rng_uni01(rng *self, PyObject *args, PyObject *kwds);
PyObject *rng_normal(rng *self,  PyObject *args, PyObject *kwds);

static PyMemberDef rng_members[] = {
  {NULL}
};
static PyMethodDef rng_methods[] = {
  {"rand",   (PyCFunction)rng_rand,   METH_VARARGS | METH_KEYWORDS, "return a uniform [0;1<<64-1] int"},
  {"uni01",  (PyCFunction)rng_uni01,  METH_VARARGS | METH_KEYWORDS, "return a uniform [0,1] floating point number"},
  {"normal", (PyCFunction)rng_normal, METH_VARARGS | METH_KEYWORDS, "return a normally distributed floating point number"},
  {NULL}
};
static PyTypeObject pyxoroshiro_rngType = {
  PyVarObject_HEAD_INIT(NULL,0)
  "pyxoroshiro.rng",         //name
  sizeof(rng),   //basicsize
  0,                         /* tp_itemsize */
  0,                         /* tp_dealloc */
  0,                         /* tp_print */
  0,                         /* tp_getattr */
  0,                         /* tp_setattr */
  0,                         /* tp_reserved */
  0,                         /* tp_repr */
  0,                         /* tp_as_number */
  0,                         /* tp_as_sequence */
  0,                         /* tp_as_mapping */
  0,                         /* tp_hash  */
  0,                         /* tp_call */
  0,                         /* tp_str */
  0,                         /* tp_getattro */
  0,                         /* tp_setattro */
  0,                         /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
  "Random Number Generator",           /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  rng_methods,             /* tp_methods */
  rng_members,             /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)rng_init,      /* tp_init */
  0,                         /* tp_alloc */
  rng_new,                 /* tp_new */
};
static PyMethodDef PyxoroshiroMethods[] = {
  {"rand",  pyxoroshiro_rand, METH_VARARGS,
   "Give a uniform 64 bit uint."},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};
static struct PyModuleDef pyxoroshiromodule = {
  PyModuleDef_HEAD_INIT,
  "pyxoroshiro",   /* name of module */
  "XOROSHIRO128+ module", //pyxoroshiro_doc, /* module documentation, may be NULL */
  -1,       /* size of per-interpreter state of the module,
               or -1 if the module keeps state in global variables. */
  NULL, NULL, NULL, NULL, NULL
};
#endif
