# $FreeBSD$

PORTNAME=	purescript
PORTVERSION=	0.13.3
CATEGORIES=	lang

MAINTAINER=	haskell@FreeBSD.org
COMMENT=	Functional language that compiles to JavaScript

LICENSE=	BSD3CLAUSE

USES=		cabal

# workaround cabal-install bug
MAKE_ENV+=	PATH=${WRKSRC}/dist-newstyle/build/${CABAL_ARCH}-freebsd/ghc-8.6.5/happy-${HAPPY_VERSION}/build/happy/:${PATH}

USE_LOCALE=	en_US.UTF-8

USE_CABAL=	Glob-0.9.3 \
		SHA-1.6.4.4 \
		aeson-1.4.4.0 \
		aeson-better-errors-0.9.1.0 \
		aeson-pretty-0.8.7 \
		alex-3.2.4 \
		ansi-terminal-0.8.2 \
		ansi-wl-pprint-0.6.8.2_2 \
		appar-0.1.8 \
		async-2.2.2 \
		attoparsec-0.13.2.2_3 \
		auto-update-0.1.6 \
		base-compat-0.10.5 \
		base-orphans-0.8.1 \
		base64-bytestring-1.0.0.2 \
		blaze-builder-0.4.1.0 \
		blaze-html-0.9.1.1 \
		blaze-markup-0.8.2.2_1 \
		bower-json-1.0.0.1 \
		boxes-0.1.5 \
		bsb-http-chunked-0.0.0.4 \
		byteorder-1.0.4 \
		bytestring-builder-0.10.8.2.0 \
		case-insensitive-1.2.1.0 \
		cheapskate-0.1.1.1_1 \
		clock-0.7.2 \
		colour-2.3.5 \
		conduit-1.3.1.1 \
		conduit-extra-1.3.4 \
		constraints-0.11 \
		css-text-0.1.3.0 \
		data-default-0.7.1.1 \
		data-default-class-0.1.2.0 \
		data-default-instances-containers-0.0.1 \
		data-default-instances-dlist-0.0.1 \
		data-default-instances-old-locale-0.0.1 \
		data-ordlist-0.4.7.0 \
		dlist-0.8.0.7 \
		easy-file-0.2.2 \
		edit-distance-0.2.2.1_1 \
		entropy-0.4.1.4_1 \
		exceptions-0.10.2 \
		fast-logger-2.4.16 \
		file-embed-0.0.11 \
		fsnotify-0.3.0.1_1 \
		gitrev-1.3.1 \
		happy-${HAPPY_VERSION} \
		hashable-1.2.7.0_1 \
		hsc2hs-0.68.4 \
		http-date-0.0.8 \
		http-types-0.12.3 \
		http2-2.0.3 \
		integer-logarithms-1.0.3_1 \
		iproute-1.7.7 \
		language-javascript-0.6.0.13 \
		lifted-async-0.10.0.4 \
		lifted-base-0.2.3.12 \
		microlens-0.4.10 \
		microlens-ghc-0.4.10 \
		microlens-mtl-0.1.11.1 \
		microlens-platform-0.3.11_1 \
		microlens-th-0.4.2.3_1 \
		monad-control-1.0.2.3 \
		monad-logger-0.3.30 \
		monad-loops-0.4.3 \
		mono-traversable-1.0.11.0 \
		mtl-compat-0.2.2 \
		network-3.0.1.1 \
		network-byte-order-0.1.1.0 \
		network-uri-2.6.1.0_1 \
		old-locale-1.0.0.7_2 \
		old-time-1.1.0.3_2 \
		optparse-applicative-0.14.3.0_2 \
		parallel-3.2.2.0 \
		pattern-arrows-0.0.2 \
		primitive-0.7.0.0 \
		protolude-0.2.3 \
		psqueues-0.2.7.2 \
		random-1.1_1 \
		regex-base-0.93.2_1 \
		regex-tdfa-1.2.3.2 \
		resourcet-1.2.2 \
		safe-0.3.17 \
		scientific-0.3.6.2 \
		semigroups-0.18.5_1 \
		simple-sendfile-0.2.28_2 \
		sourcemap-0.1.6_1 \
		split-0.2.3.3_1 \
		stm-chans-3.0.0.4_1 \
		streaming-commons-0.2.1.1 \
		stringsearch-0.3.6.6_1 \
		syb-0.7.1 \
		tagged-0.8.6_1 \
		tagsoup-0.14.8 \
		th-abstraction-0.3.1.0 \
		time-compat-1.9.2.2_1 \
		time-manager-0.0.0 \
		transformers-base-0.4.5.2 \
		transformers-compat-0.6.5 \
		typed-process-0.2.6.0 \
		uniplate-1.6.12_1 \
		unix-compat-0.5.2 \
		unix-time-0.4.7 \
		unliftio-core-0.1.2.0_2 \
		unordered-containers-0.2.10.0_1 \
		utf8-string-1.0.1.1_3 \
		uuid-types-1.0.3_2 \
		vault-0.3.1.3 \
		vector-0.12.0.3_2 \
		vector-algorithms-0.8.0.1_2 \
		void-0.7.3 \
		wai-3.2.2.1 \
		wai-websockets-3.0.1.2 \
		warp-3.3.0 \
		websockets-0.12.5.3 \
		word8-0.1.3 \
		xss-sanitize-0.3.6 \
		zlib-0.6.2

EXECUTABLES=	purs
CABAL_FLAGS=	release
HAPPY_VERSION=	1.19.9

.include <bsd.port.mk>
