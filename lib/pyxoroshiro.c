#include <Python.h>
#include <numpy/arrayobject.h>
#include "pyxoroshiro/xoroshiro-variates.h"
#include "pyxoroshiro/pyxoroshiro.h"
PyObject* pyxoroshiro_rand(PyObject *self, PyObject *args) {
  return PyLong_FromLong(4); // chosen by fair dice roll
}
PyMODINIT_FUNC
PyInit_pyxoroshiro(void)
{
  PyObject *m;

  m = PyModule_Create(&pyxoroshiromodule);
  if (m == NULL)
    return NULL;

  PyxoroshiroError = PyErr_NewException("pyxoroshiro.error", NULL, NULL);
  Py_INCREF(PyxoroshiroError);
  PyModule_AddObject(m, "error", PyxoroshiroError);

  if(PyType_Ready(&pyxoroshiro_rngType) < 0) {
    return NULL;
  }
  Py_INCREF(&pyxoroshiro_rngType);
  PyModule_AddObject(m, "rng", (PyObject*)&pyxoroshiro_rngType);
  import_array();

  return m;
}

PyObject *rng_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
  rng *self;
  self = (rng*)type->tp_alloc(type,0);
  return (PyObject*)self;
}

int rng_init(rng *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"seed", NULL};
  uint64_t seed;
  if(! PyArg_ParseTupleAndKeywords(args, kwds, "K", kwlist, &seed))
    return -1;
  xoroshiro128p_seed(self->state, seed);
  return 0;
}

PyObject *rng_rand(rng *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"num", NULL};
  signed long num = 1;
  if(! PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &num))
    return NULL;
  if(num <= 0){
    PyErr_SetString(PyExc_ValueError, "size of array must be positive.");
    return NULL;
  }
  if(num == 1){
    return PyLong_FromUnsignedLongLong(xoroshiro128p_next(self->state));
  } else {
    PyObject *array = PyArray_SimpleNew(1, &num, NPY_UINT64);
    if (array == NULL) {
      PyErr_SetString(PyExc_MemoryError, "Couldn't get new numpy array.");
      return NULL;
    }
    float *samples = PyArray_DATA(array);
    for(long i = 0; i < num; i++) {
      samples[i] = xoroshiro128p_next(self->state);
    }
    return array;
  }
}

PyObject *rng_uni01(rng *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"num", NULL};
  signed long num = 1;
  if(! PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &num))
    return NULL;
  if(num <= 0){
    PyErr_SetString(PyExc_ValueError, "size of array must be positive.");
    return NULL;
  }
  if(num == 1){
    return PyFloat_FromDouble(uint64_to_d(xoroshiro128p_next(self->state)));
  } else {
    npy_intp size = num;
    PyObject *array = PyArray_SimpleNew(1, &size, NPY_FLOAT32);
    if (array == NULL) {
      PyErr_SetString(PyExc_MemoryError, "Couldn't get new numpy array.");
      return NULL;
    }
    float *samples = PyArray_DATA(array);
    for(long i = 0; i < num; i++) {
      samples[i] = uint64_to_d(xoroshiro128p_next(self->state));
    }
    return array;
  }
}

PyObject *rng_normal(rng *self, PyObject *args, PyObject *kwds) {
  return PyFloat_FromDouble(xoroshiro128p_normalf(self->state, 32));
}
