# Created by: Mario Sergio Fujikawa Ferreira <lioux@FreeBSD.org>
# $FreeBSD$

PORTNAME=	md4coll
DISTVERSION=	0.1
CATEGORIES=	security
MASTER_SITES=	http://www.stachliu.com/ \
		http://mirror.roe.ch/dist/${PORTNAME}/${DISTVERSION}/
DISTNAME=	${PORTNAME}.c
EXTRACT_SUFX=
EXTRACT_ONLY=

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Fast MD4 collision generator

BROKEN=		unfetchable
DEPRECATED=	Unfetchable, unmaintained
EXPIRATION_DATE=	2019-10-16

NO_WRKSUBDIR=	yes
PLIST_FILES=	bin/${PORTNAME}

post-extract:
	@${CP} ${DISTDIR}/${DISTNAME} ${WRKSRC}

do-build:
	${CC} ${CFLAGS} ${WRKSRC}/${DISTNAME} -o ${WRKSRC}/${PORTNAME}

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/${PORTNAME} ${STAGEDIR}${PREFIX}/bin

.include <bsd.port.mk>
