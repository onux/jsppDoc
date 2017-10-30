set(
    EMITTER_SOURCE_FILES
        FileEmitter.cc
        FileEmitter.h
        OutputEmitter.h
)

add_library(Emitters ${EMITTER_SOURCE_FILES})
