; File: stdint.inc
; Author: Joel Souza
; Date: 2026-03-17
; Description: Standard constant integers types and masks

%ifndef STDINT_INC
%define STDINT_INC

%define INT32_BYTES	4
%define INT32_SHIFT	2
%assign INT32_MASK	INT32_BYTES - 1

%endif
