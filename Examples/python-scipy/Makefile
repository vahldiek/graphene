# Use one of the following commands to build the manifest for Python3:
#
# - make                Building for Linux
# - make DEBUG=1        Building for Linux (with Graphene debug output)
# - make SGX=1          Building for SGX
# - make SGX=1 DEBUG=1  Building for SGX (with Graphene debug output)
#
# Use `make clean` to remove Graphene-generated files.

include ../../Scripts/Makefile.configs

# Python constants are declared in Makefile.python
include ../../Scripts/Makefile.python

# Relative path to Graphene root
GRAPHENEDIR ?= ../..
SGX_SIGNER_KEY ?= $(GRAPHENEDIR)/Pal/src/host/Linux-SGX/signer/enclave-key.pem

ifeq ($(DEBUG),1)
GRAPHENE_LOG_LEVEL = debug
else
GRAPHENE_LOG_LEVEL = error
endif

.PHONY: all
all: python.manifest pal_loader
ifeq ($(SGX),1)
all: python.manifest.sgx python.sig python.token
endif

python.manifest: python.manifest.template
	sed -e 's|$$(GRAPHENEDIR)|'"$(GRAPHENEDIR)"'|g' \
	    -e 's|$$(GRAPHENE_LOG_LEVEL)|'"$(GRAPHENE_LOG_LEVEL)"'|g' \
	    -e 's|$$(PYTHONDISTHOME)|'"$(PYTHONDISTHOME)"'|g' \
	    -e 's|$$(PYTHONHOME)|'"$(PYTHONHOME)"'|g' \
	    -e 's|$$(PYTHONEXEC)|'"$(PYTHONEXEC)"'|g' \
	    -e 's|$$(ARCH_LIBDIR)|'"$(ARCH_LIBDIR)"'|g' \
	    -e 's|$$(PYTHONSHORTVERSION)|'"$(PYTHONSHORTVERSION)"'|g' \
	    -e 's|$$(PYTHON_ARCH_LONG)|'"$(PYTHON_ARCH_LONG)"'|g' \
	    -e 's|$$(SYS)|'"$(SYS)"'|g' \
	    $< >$@

# Python manifests for SGX:
#   Generating the SGX-specific manifest (python.manifest.sgx), the enclave signature,
#   and the token for enclave initialization.

python.manifest.sgx: python.manifest
	$(GRAPHENEDIR)/Pal/src/host/Linux-SGX/signer/pal-sgx-sign \
		-libpal $(GRAPHENEDIR)/Runtime/libpal-Linux-SGX.so \
		-key $(SGX_SIGNER_KEY) \
		-manifest python.manifest \
		-output $@

python.sig: python.manifest.sgx

python.token: python.sig
	$(GRAPHENEDIR)/Pal/src/host/Linux-SGX/signer/pal-sgx-get-token -output $@ -sig $<

pal_loader:
	ln -s $(GRAPHENEDIR)/Runtime/pal_loader $@

.PHONY: check
check: all
	./pal_loader ./python scripts/test-numpy.py > OUTPUT 2> /dev/null
	@sleep 1  # to make sure Bash child processes flush output under Graphene-SGX
	@grep -q "dot: " OUTPUT && echo "[ Success 1/2 ]"
	@rm OUTPUT

	./pal_loader ./python scripts/test-scipy.py > OUTPUT 2> /dev/null
	@sleep 1
	@grep -q "cholesky: " OUTPUT && echo "[ Success 2/2 ]"
	@rm OUTPUT

.PHONY: clean
clean:
	$(RM) *.manifest *.manifest.sgx *.token *.sig pal_loader OUTPUT*
	$(RM) -r scripts/__pycache__

.PHONY: distclean
distclean: clean
