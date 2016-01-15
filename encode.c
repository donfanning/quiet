#include "encode.h"

encoder_options *get_encoder_profile(json_t *root, const char *profilename) {
    json_t *profile = json_object_get(root, profilename);
    if (!profile) {
        printf("failed to access profile %s\n", profilename);
        return NULL;
    }

    encoder_options *opt = calloc(1, sizeof(encoder_options));
    if (!opt) {
        printf("allocation of encoder_options failed\n");
        return NULL;
    }

    json_t *v;
    if ((v = json_object_get(profile, "checksum_scheme"))) {
        const char *scheme = json_string_value(v);
        opt->checksum_scheme = liquid_getopt_str2crc(scheme);
    }
    if ((v = json_object_get(profile, "inner_fec_scheme"))) {
        const char *scheme = json_string_value(v);
        opt->inner_fec_scheme = liquid_getopt_str2fec(scheme);
    }
    if ((v = json_object_get(profile, "outer_fec_scheme"))) {
        const char *scheme = json_string_value(v);
        opt->outer_fec_scheme = liquid_getopt_str2fec(scheme);
    }
    if ((v = json_object_get(profile, "mod_scheme"))) {
        const char *scheme = json_string_value(v);
        opt->mod_scheme = liquid_getopt_str2mod(scheme);
    }
    if ((v = json_object_get(profile, "frame_length"))) {
        opt->frame_len = json_integer_value(v);
    }
    if ((v = json_object_get(profile, "ofdm"))) {
        json_t *vv;
        opt->is_ofdm = true;
        if ((vv = json_object_get(v, "num_subcarriers"))) {
            opt->ofdmopt.num_subcarriers = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "cyclic_prefix_length"))) {
            opt->ofdmopt.cyclic_prefix_len = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "taper_length"))) {
            opt->ofdmopt.taper_len = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "left_band"))) {
            opt->ofdmopt.left_band = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "right_band"))) {
            opt->ofdmopt.right_band = json_integer_value(vv);
        }
    } else {
        opt->is_ofdm = false;
    }
    if ((v = json_object_get(profile, "modulation"))) {
        json_t *vv;
        if ((vv = json_object_get(v, "samples_per_symbol"))) {
            opt->modopt.samples_per_symbol = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "symbol_delay"))) {
            opt->modopt.symbol_delay = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "excess_bandwidth"))) {
            opt->modopt.excess_bw = json_real_value(vv);
        }
        if ((vv = json_object_get(v, "center_radians"))) {
            opt->modopt.center_rads = json_real_value(vv);
        }
        if ((vv = json_object_get(v, "gain"))) {
            opt->modopt.gain = json_real_value(vv);
        }
    }
    if ((v = json_object_get(profile, "encoder_filters"))) {
        json_t *vv;
        json_t *vvv;
        if ((vv = json_object_get(v, "premix"))) {
            if ((vvv = json_object_get(vv, "cutoff"))) {
                opt->modopt.premix_filter_opt.cutoff = json_real_value(vvv);
            }
            if ((vvv = json_object_get(vv, "order"))) {
                opt->modopt.premix_filter_opt.order = json_integer_value(vvv);
            }
        }
        if ((vv = json_object_get(v, "dc_filter_alpha"))) {
            opt->modopt.dc_filter_opt.alpha = json_real_value(vv);
        }
    }

    return opt;
}

encoder_options *get_encoder_profile_file(const char *fname,
                                          const char *profilename) {
    json_error_t error;
    json_t *root = json_load_file(fname, 0, &error);

    if (!root) {
        printf("failed to read profiles\n");
        return NULL;
    }

    return get_encoder_profile(root, profilename);
}

encoder_options *get_encoder_profile_str(const char *input,
                                         const char *profilename) {
    json_error_t error;
    json_t *root = json_loads(input, 0, &error);

    if (!root) {
        printf("failed to read profiles\n");
        return NULL;
    }

    return get_encoder_profile(root, profilename);
}

decoder_options *get_decoder_profile(json_t *root, const char *profilename) {
    json_t *profile = json_object_get(root, profilename);
    if (!profile) {
        printf("failed to access profile %s\n", profilename);
        return NULL;
    }

    decoder_options *opt = calloc(1, sizeof(decoder_options));
    if (!opt) {
        printf("allocation of decoder_options failed\n");
        return NULL;
    }

    json_t *v;
    if ((v = json_object_get(profile, "ofdm"))) {
        json_t *vv;
        opt->is_ofdm = true;
        if ((vv = json_object_get(v, "num_subcarriers"))) {
            opt->ofdmopt.num_subcarriers = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "cyclic_prefix_length"))) {
            opt->ofdmopt.cyclic_prefix_len = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "taper_length"))) {
            opt->ofdmopt.taper_len = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "left_band"))) {
            opt->ofdmopt.left_band = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "right_band"))) {
            opt->ofdmopt.right_band = json_integer_value(vv);
        }
    } else {
        opt->is_ofdm = false;
    }
    if ((v = json_object_get(profile, "modulation"))) {
        json_t *vv;
        if ((vv = json_object_get(v, "samples_per_symbol"))) {
            opt->demodopt.samples_per_symbol = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "symbol_delay"))) {
            opt->demodopt.symbol_delay = json_integer_value(vv);
        }
        if ((vv = json_object_get(v, "excess_bandwidth"))) {
            opt->demodopt.excess_bw = json_real_value(vv);
        }
        if ((vv = json_object_get(v, "center_radians"))) {
            opt->demodopt.center_rads = json_real_value(vv);
        }
    }
    if ((v = json_object_get(profile, "decoder_filters"))) {
        json_t *vv;
        json_t *vvv;
        if ((vv = json_object_get(v, "mix"))) {
            if ((vvv = json_object_get(vv, "cutoff"))) {
                opt->demodopt.mix_filter_opt.cutoff = json_real_value(vvv);
            }
            if ((vvv = json_object_get(vv, "order"))) {
                opt->demodopt.mix_filter_opt.order = json_integer_value(vvv);
            }
        }
    }

    return opt;
}

decoder_options *get_decoder_profile_file(const char *fname,
                                          const char *profilename) {
    json_error_t error;
    json_t *root = json_load_file(fname, 0, &error);

    if (!root) {
        printf("failed to read profiles\n");
        return NULL;
    }

    return get_decoder_profile(root, profilename);
}

decoder_options *get_decoder_profile_str(const char *input,
                                         const char *profilename) {
    json_error_t error;
    json_t *root = json_loads(input, 0, &error);

    if (!root) {
        printf("failed to read profiles\n");
        return NULL;
    }

    return get_decoder_profile(root, profilename);
}

unsigned char *create_ofdm_subcarriers(const ofdm_options *opt) {
    unsigned char *subcarriers =
        malloc(opt->num_subcarriers * sizeof(unsigned char));
    // get the default subcarrier placement and then modify it slightly
    ofdmframe_init_default_sctype(opt->num_subcarriers, subcarriers);
    // now add some nulls
    size_t left_end = opt->num_subcarriers / 2;
    while (subcarriers[left_end] == OFDMFRAME_SCTYPE_NULL) {
        left_end--;
    }
    size_t right_end = opt->num_subcarriers / 2;
    while (subcarriers[right_end] == OFDMFRAME_SCTYPE_NULL) {
        right_end++;
    }
    // n.b. confusingly the left part of the array corresponds to the right band
    // and vice versa
    for (size_t i = 0; i < opt->right_band; i++) {
        subcarriers[left_end - i] = OFDMFRAME_SCTYPE_NULL;
    }
    for (size_t i = 0; i < opt->left_band; i++) {
        subcarriers[right_end + i] = OFDMFRAME_SCTYPE_NULL;
    }

    return subcarriers;
}

modulator *create_modulator(const modulator_options *opt) {
    if (!opt) {
        return NULL;
    }

    modulator *m = malloc(sizeof(modulator));

    m->opt = *opt;

    m->nco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(m->nco, 0.0f);
    nco_crcf_set_frequency(m->nco, opt->center_rads);

    /*
    size_t num_coeffs = (2 * opt->samples_per_symbol * opt->symbol_delay) + 1;
    float coeff[num_coeffs];
    liquid_firdes_rcos(opt->samples_per_symbol, opt->symbol_delay,
                        opt->excess_bw, 0, coeff);
    m->interp = firinterp_crcf_create(opt->samples_per_symbol, coeff,
    num_coeffs);
    */
    if (opt->samples_per_symbol > 1) {
        m->interp = firinterp_crcf_create_kaiser(opt->samples_per_symbol,
                                                 opt->symbol_delay, 60.0f);
    } else {
        m->opt.samples_per_symbol = 1;
        m->opt.symbol_delay = 0;
        m->interp = NULL;
    }

    if (opt->premix_filter_opt.order) {
        filter_options filteropt = opt->premix_filter_opt;
        m->premixfilter =
            iirfilt_crcf_create_lowpass(filteropt.order, filteropt.cutoff);
    } else {
        m->premixfilter = NULL;
    }
    m->mixfilter = firfilt_crcf_create_kaiser(
        2048, opt->center_rads * (1 / (2 * M_PI)) + 0.23f, 60.0f, 0);
    if (opt->dc_filter_opt.alpha) {
        m->dcfilter = iirfilt_crcf_create_dc_blocker(opt->dc_filter_opt.alpha);
    } else {
        m->dcfilter = NULL;
    }

    return m;
}

size_t modulate_sample_len(const modulator *m, size_t symbol_len) {
    if (!m) {
        return 0;
    }

    return m->opt.samples_per_symbol * symbol_len;
}

// modulate assumes that samples is large enough to store symbol_len *
// samples_per_symbol samples
// returns number of samples written to *samples
size_t modulate(modulator *m, const float complex *symbols, size_t symbol_len,
                sample_t *samples) {
    if (!m) {
        return 0;
    }

    float complex post_filter[m->opt.samples_per_symbol];
    size_t written = 0;
    for (size_t i = 0; i < symbol_len; i++) {
        if (m->interp) {
            firinterp_crcf_execute(m->interp, symbols[i], &post_filter[0]);
        } else {
            post_filter[0] = symbols[i];  // pass thru
        }
        for (size_t j = 0; j < m->opt.samples_per_symbol; j++) {
            float complex mixed;
            if (m->premixfilter) {
                iirfilt_crcf_execute(m->premixfilter, post_filter[j],
                                     &post_filter[j]);
            }
            nco_crcf_mix_up(m->nco, post_filter[j], &mixed);
            // firfilt_crcf_push(m->mixfilter, mixed);
            // firfilt_crcf_execute(m->mixfilter, &filtered);
            if (m->dcfilter) {
                iirfilt_crcf_execute(m->dcfilter, mixed, &mixed);
            }
            samples[i * m->opt.samples_per_symbol + j] =
                crealf(mixed) * m->opt.gain;
            written++;
            nco_crcf_step(m->nco);
        }
    }
    return written;
}

size_t modulate_flush_sample_len(modulator *m) {
    if (!m) {
        return 0;
    }

    return m->opt.samples_per_symbol * 2 * m->opt.symbol_delay;
}

size_t modulate_flush(modulator *m, sample_t *samples) {
    if (!m) {
        return 0;
    }

    if (!m->opt.symbol_delay) {
        return 0;
    }

    size_t symbol_len = 2 * m->opt.symbol_delay;
    float complex terminate[symbol_len];
    for (size_t i = 0; i < symbol_len; i++) {
        terminate[i] = 0;
    }

    return modulate(m, terminate, symbol_len, samples);
}

void destroy_modulator(modulator *m) {
    if (!m) {
        return;
    }

    nco_crcf_destroy(m->nco);
    if (m->interp) {
        firinterp_crcf_destroy(m->interp);
    }
    if (m->premixfilter) {
        iirfilt_crcf_destroy(m->premixfilter);
    }
    firfilt_crcf_destroy(m->mixfilter);
    if (m->dcfilter) {
        iirfilt_crcf_destroy(m->dcfilter);
    }
    free(m);
}

demodulator *create_demodulator(const demodulator_options *opt) {
    if (!opt) {
        return NULL;
    }

    demodulator *d = malloc(sizeof(demodulator));

    d->opt = *opt;

    d->nco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(d->nco, 0.0f);
    nco_crcf_set_frequency(d->nco, opt->center_rads);

    /*
    size_t num_coeffs = (2 * opt->samples_per_symbol * opt->symbol_delay) + 1;
    float coeff_rev[num_coeffs], coeff[num_coeffs];
    liquid_firdes_rcos(opt->samples_per_symbol, opt->symbol_delay,
                        opt->excess_bw, 0, coeff_rev);
    for (size_t i = 0; i < num_coeffs; i++) {
        coeff[i] = coeff_rev[num_coeffs - i - 1];
    }
    d->decim = firdecim_crcf_create(opt->samples_per_symbol, coeff, num_coeffs);
    */
    if (opt->samples_per_symbol > 1) {
        d->decim = firdecim_crcf_create_kaiser(opt->samples_per_symbol,
                                               opt->symbol_delay, 60.0f);
    } else {
        d->opt.samples_per_symbol = 1;
        d->opt.symbol_delay = 0;
        d->decim = NULL;
    }

    /*
    d->premixfilter = iirfilt_crcf_create_lowpass(3, 0.5f - bandedge);
    */
    d->premixfilter = NULL;
    if (opt->mix_filter_opt.order) {
        filter_options filteropt = opt->mix_filter_opt;
        d->mixfilter =
            iirfilt_crcf_create_lowpass(filteropt.order, filteropt.cutoff);
    } else {
        d->mixfilter = NULL;
    }
    return d;
}

size_t demodulate_symbol_len(const demodulator *d, size_t sample_len) {
    if (!d) {
        return 0;
    }

    if (sample_len % d->opt.samples_per_symbol != 0) {
        printf("must receive multiple of samples_per_symbol samples");
        return 0;
    }

    return sample_len / d->opt.samples_per_symbol;
}

size_t demodulate(demodulator *d, sample_t *samples, size_t sample_len,
                  float complex *symbols) {
    if (!d) {
        return 0;
    }

    if (sample_len % d->opt.samples_per_symbol != 0) {
        printf("must receive multiple of samples_per_symbol samples");
        return 0;
    }

    float complex post_mixer[d->opt.samples_per_symbol];
    size_t written = 0;
    for (size_t i = 0; i < sample_len; i += d->opt.samples_per_symbol) {
        for (size_t j = 0; j < d->opt.samples_per_symbol; j++) {
            nco_crcf_mix_down(d->nco, samples[i + j], &post_mixer[j]);
            if (d->mixfilter) {
                iirfilt_crcf_execute(d->mixfilter, post_mixer[j],
                                     &post_mixer[j]);
            }
            nco_crcf_step(d->nco);
        }

        if (d->decim) {
            firdecim_crcf_execute(d->decim, &post_mixer[0],
                                  &symbols[(i / d->opt.samples_per_symbol)]);
            symbols[(i / d->opt.samples_per_symbol)] /=
                d->opt.samples_per_symbol;
        } else {
            symbols[i] = post_mixer[0];
        }
        written++;
    }

    return written;
}

size_t demodulate_flush_symbol_len(demodulator *d) {
    if (!d) {
        return 0;
    }

    return 2 * d->opt.symbol_delay;
}

size_t demodulate_flush(demodulator *d, float complex *symbols) {
    if (!d) {
        return 0;
    }

    size_t sample_len = 2 * d->opt.symbol_delay * d->opt.samples_per_symbol;
    sample_t terminate[sample_len];
    for (size_t i = 0; i < sample_len; i++) {
        terminate[i] = 0;
    }

    return demodulate(d, terminate, sample_len, symbols);
}

void destroy_demodulator(demodulator *d) {
    if (!d) {
        return;
    }

    nco_crcf_destroy(d->nco);
    if (d->decim) {
        firdecim_crcf_destroy(d->decim);
    }
    if (d->premixfilter) {
        iirfilt_crcf_destroy(d->premixfilter);
    }
    if (d->mixfilter) {
        iirfilt_crcf_destroy(d->mixfilter);
    }
    free(d);
}

void create_ofdm_encoder(const encoder_options *opt, encoder *e) {
    ofdm_encoder ofdm;

    ofdmflexframegenprops_s props = {
        .check = opt->checksum_scheme,
        .fec0 = opt->inner_fec_scheme,
        .fec1 = opt->outer_fec_scheme,
        .mod_scheme = opt->mod_scheme,
    };

    unsigned char *subcarriers = create_ofdm_subcarriers(&opt->ofdmopt);
    ofdm.framegen = ofdmflexframegen_create(
        opt->ofdmopt.num_subcarriers, opt->ofdmopt.cyclic_prefix_len,
        opt->ofdmopt.taper_len, subcarriers, &props);

    size_t symbolbuf_len =
        opt->ofdmopt.num_subcarriers + opt->ofdmopt.cyclic_prefix_len;
    e->symbolbuf = malloc(symbolbuf_len *
                          sizeof(float complex));  // XXX check malloc result
    e->symbolbuf_len = symbolbuf_len;

    free(subcarriers);

    e->frame.ofdm = ofdm;
}

void create_modem_encoder(const encoder_options *opt, encoder *e) {
    modem_encoder modem;

    flexframegenprops_s props = {
        .check = opt->checksum_scheme,
        .fec0 = opt->inner_fec_scheme,
        .fec1 = opt->outer_fec_scheme,
        .mod_scheme = opt->mod_scheme,
    };

    modem.framegen = flexframegen_create(&props);

    e->symbolbuf = NULL;
    e->symbolbuf_len = 0;
    modem.symbols_remaining = 0;

    e->frame.modem = modem;
}

encoder *create_encoder(const encoder_options *opt) {
    if (!opt) {
        return NULL;
    }

    encoder *e = malloc(sizeof(encoder));

    e->opt = *opt;

    printf("%d %d\n", opt->checksum_scheme, opt->mod_scheme);
    printf("%f %f\n", opt->modopt.excess_bw, opt->modopt.center_rads);

    if (opt->is_ofdm) {
        create_ofdm_encoder(opt, e);
    } else {
        create_modem_encoder(opt, e);
    }

    e->mod = create_modulator(&(opt->modopt));

    e->samplebuf_cap = modulate_sample_len(e->mod, e->symbolbuf_len);
    e->samplebuf = malloc(e->samplebuf_cap * sizeof(sample_t));
    e->samplebuf_len = 0;
    e->samplebuf_offset = 0;

    e->payload = NULL;
    e->payload_length = 0;
    e->has_flushed = true;

    return e;
}

int _encoder_assembled(encoder *e) {
    if (e->opt.is_ofdm) {
        return ofdmflexframegen_is_assembled(e->frame.ofdm.framegen);
    } else {
        return flexframegen_is_assembled(e->frame.modem.framegen);
    }
}

int encoder_set_payload(encoder *e, uint8_t *payload, size_t payload_length) {
    int had_payload = (e->payload_length != 0) || (_encoder_assembled(e)) ||
                      (e->samplebuf_len != 0);

    e->payload = payload;
    e->payload_length = payload_length;
    e->samplebuf_len = 0;
    e->samplebuf_offset = 0;
    e->has_flushed = false;

    if (e->opt.is_ofdm) {
        ofdmflexframegen_reset(e->frame.ofdm.framegen);
    } else {
        flexframegen_reset(e->frame.modem.framegen);
        e->frame.modem.symbols_remaining = 0;
    }

    return had_payload;
}

void _encoder_consume(encoder *e) {
    size_t payload_length = e->opt.frame_len;
    if (e->payload_length < payload_length) {
        payload_length = e->payload_length;
    }
    if (e->opt.is_ofdm) {
        uint8_t *header = calloc(sizeof(uint8_t), 8);
        ofdmflexframegen_assemble(e->frame.ofdm.framegen, header, e->payload,
                                  payload_length);
        free(header);
    } else {
        uint8_t *header = calloc(sizeof(uint8_t), 14);
        flexframegen_assemble(e->frame.modem.framegen, header, e->payload,
                              payload_length);
        e->frame.modem.symbols_remaining =
            flexframegen_getframelen(e->frame.modem.framegen);
        free(header);
    }

    e->payload += payload_length;
    e->payload_length -= payload_length;
}

size_t encoder_sample_len(encoder *e, size_t data_len) {
    uint8_t *empty = calloc(data_len, sizeof(uint8_t));
    uint8_t header[8];
    if (e->opt.is_ofdm) {
        ofdmflexframegen_assemble(e->frame.ofdm.framegen, header, empty,
                                  data_len);  // TODO actual calculation?
        size_t num_ofdm_blocks =
            ofdmflexframegen_getframelen(e->frame.ofdm.framegen);
        return modulate_sample_len(e->mod, num_ofdm_blocks * e->symbolbuf_len);
    } else {
        flexframegen_assemble(e->frame.modem.framegen, header, empty, data_len);
        size_t num_symbols = flexframegen_getframelen(e->frame.modem.framegen);
        return modulate_sample_len(e->mod, num_symbols);
    }
}

size_t _constrained_write(sample_t *src, size_t src_len, sample_t *dst,
                          size_t dest_len) {
    size_t len = src_len;
    if (dest_len < src_len) {
        len = dest_len;
    }

    memmove(dst, src, len * sizeof(sample_t));

    return len;
}

size_t _encoder_pad(encoder *e) {
    size_t padding_len;
    if (e->opt.is_ofdm) {
        padding_len =
            e->opt.ofdmopt.cyclic_prefix_len * e->mod->opt.samples_per_symbol;
    } else {
        padding_len = 32;
    }
    if (padding_len > e->samplebuf_cap) {
        e->samplebuf =
            realloc(e->samplebuf,
                    padding_len * sizeof(sample_t));  // XXX check malloc result
        e->samplebuf_cap = padding_len;
    }
    for (size_t i = 0; i < padding_len; i++) {
        e->samplebuf[i] = 0;
    }
    return padding_len;
}

size_t _encoder_fillsymbols(encoder *e, size_t requested_length) {
    if (e->opt.is_ofdm) {
        // ofdm can't control the size of its blocks, so it ignores
        // requested_length
        ofdmflexframegen_writesymbol(e->frame.ofdm.framegen, e->symbolbuf);
        return e->symbolbuf_len;
    } else {
        if (requested_length > e->frame.modem.symbols_remaining) {
            requested_length = e->frame.modem.symbols_remaining;
        }

        if (requested_length > e->symbolbuf_len) {
            e->symbolbuf =
                realloc(e->symbolbuf,
                        requested_length *
                            sizeof(float complex));  // XXX check malloc result
            e->symbolbuf_len = requested_length;
        }

        flexframegen_write_samples(e->frame.modem.framegen, e->symbolbuf,
                                   requested_length);
        e->frame.modem.symbols_remaining -= requested_length;
        return requested_length;
    }
}

size_t encode(encoder *e, sample_t *samplebuf, size_t samplebuf_len) {
    if (!e) {
        return 0;
    }

    size_t written = 0;
    while (written < samplebuf_len) {
        size_t remaining = samplebuf_len - written;
        size_t iter_written;

        if (e->samplebuf_len > 0) {
            iter_written =
                _constrained_write(e->samplebuf + e->samplebuf_offset,
                                   e->samplebuf_len, samplebuf, remaining);
            samplebuf += iter_written;
            written += iter_written;
            e->samplebuf_offset += iter_written;
            e->samplebuf_len -= iter_written;
            continue;
        }

        e->samplebuf_offset = 0;

        if (!(_encoder_assembled(e))) {
            if (e->payload_length == 0) {
                if (e->has_flushed) {
                    break;
                }
                // TODO make modulator understand interp/decim and set flush len
                // to 0 accordingly
                e->samplebuf_len = modulate_flush(e->mod, e->samplebuf);
                e->has_flushed = true;
                continue;
            } else {
                e->samplebuf_len = _encoder_pad(e);
            }
            _encoder_consume(e);
            continue;
        }

        // now we get to the steady state, writing one block of symbols at a
        // time
        // we provide symbols_wanted as a hint of how many symbols to get
        size_t symbols_wanted = remaining / e->mod->opt.samples_per_symbol;
        if (remaining % e->mod->opt.samples_per_symbol) {
            symbols_wanted++;
        }
        size_t symbols_written = _encoder_fillsymbols(e, symbols_wanted);
        size_t sample_buffer_needed =
            symbols_written * e->mod->opt.samples_per_symbol;

        if (remaining < sample_buffer_needed) {
            // in this case, just write to the samplebuf and restart loop
            // TODO call modulate once for remaining samples and then for the
            // rest

            if (sample_buffer_needed > e->samplebuf_cap) {
                e->samplebuf =
                    realloc(e->samplebuf,
                            sample_buffer_needed *
                                sizeof(sample_t));  // XXX check malloc result
                e->samplebuf_cap = sample_buffer_needed;
            }

            e->samplebuf_len =
                modulate(e->mod, e->symbolbuf, symbols_written, e->samplebuf);
            continue;
        }

        // we have enough capacity in the output buf, so just write there
        // directly
        iter_written =
            modulate(e->mod, e->symbolbuf, symbols_written, samplebuf);
        samplebuf += iter_written;
        written += iter_written;
    }

    return written;
}

void destroy_encoder(encoder *e) {
    if (!e) {
        return;
    }

    if (e->opt.is_ofdm) {
        ofdmflexframegen_destroy(e->frame.ofdm.framegen);
    } else {
        flexframegen_destroy(e->frame.modem.framegen);
    }
    destroy_modulator(e->mod);
    free(e->symbolbuf);
    free(e->samplebuf);
    free(e);
}

const size_t decoder_writebuf_initial_len = 2 << 12;

int _decoder_resize_buffer(decoder *d) {
    if (!d) {
        return 1;
    }

    size_t newlen = d->writebuf_len * 2;
    uint8_t *newbuf = realloc(d->writebuf, newlen);
    if (!newbuf) {
        return 1;
    }

    d->writebuf = newbuf;
    d->writebuf_len = newlen;

    return 0;
}

int _on_decode(unsigned char *header, int header_valid, unsigned char *payload,
               unsigned int payload_len, int payload_valid,
               framesyncstats_s stats, void *dvoid) {
    if (!header_valid || !payload_valid) {
        // XXX
        return 1;
    }

    if (!dvoid) {
        return 0;
    }

    decoder *d = dvoid;

    while (payload_len > (d->writebuf_len - d->writebuf_accum)) {
        if (!_decoder_resize_buffer(d)) {
            return 1;
        }
    }

    memmove(d->writebuf + d->writebuf_accum, payload, payload_len);
    d->writebuf_accum += payload_len;

    return 0;
}

void create_ofdm_decoder(const decoder_options *opt, decoder *d) {
    ofdm_decoder ofdm;

    unsigned char *subcarriers = create_ofdm_subcarriers(&opt->ofdmopt);
    ofdm.framesync = ofdmflexframesync_create(
        opt->ofdmopt.num_subcarriers, opt->ofdmopt.cyclic_prefix_len,
        opt->ofdmopt.taper_len, subcarriers, _on_decode, d);
    if (opt->is_debug) {
        ofdmflexframesync_debug_enable(ofdm.framesync);
    }

    size_t symbolbuf_len =
        opt->ofdmopt.num_subcarriers + opt->ofdmopt.cyclic_prefix_len;
    d->symbolbuf = malloc(symbolbuf_len *
                          sizeof(float complex));  // XXX check malloc result
    d->symbolbuf_len = symbolbuf_len;

    free(subcarriers);

    d->frame.ofdm = ofdm;
}

void create_modem_decoder(const decoder_options *opt, decoder *d) {
    modem_decoder modem;

    modem.framesync = flexframesync_create(_on_decode, d);
    if (opt->is_debug) {
        flexframesync_debug_enable(modem.framesync);
    }

    size_t symbolbuf_len = 256;
    d->symbolbuf = malloc(symbolbuf_len * sizeof(float complex));
    d->symbolbuf_len = symbolbuf_len;

    d->frame.modem = modem;
}

decoder *create_decoder(const decoder_options *opt) {
    if (!opt) {
        return NULL;
    }

    decoder *d = malloc(sizeof(decoder));

    d->opt = *opt;

    size_t writebuf_len = decoder_writebuf_initial_len;
    uint8_t *writebuf = malloc(writebuf_len);
    d->writebuf = writebuf;
    d->writebuf_len = writebuf_len;
    d->writebuf_accum = 0;

    if (opt->is_ofdm) {
        create_ofdm_decoder(opt, d);
    } else {
        create_modem_decoder(opt, d);
    }

    d->demod = create_demodulator(&(opt->demodopt));

    d->i = 0;

    printf("decoder created\n");
    printf("is_ofdm %d\n", opt->is_ofdm);

    return d;
}

size_t decoder_readbuf(decoder *d, uint8_t *data, size_t data_len) {
    if (!d) {
        return 0;
    }

    if (data_len > d->writebuf_accum) {
        return 0;
    }

    memmove(data, d->writebuf, data_len);

    d->writebuf_accum -= data_len;
    memmove(d->writebuf, d->writebuf + data_len, d->writebuf_accum);

    return data_len;
}

size_t decode_max_len(decoder *d) {
    if (!d) {
        return 0;
    }

    return d->symbolbuf_len * d->demod->opt.samples_per_symbol;
}

// returns number of uint8_ts accumulated in buf
size_t decode(decoder *d, sample_t *samplebuf, size_t sample_len) {
    if (!d) {
        return 0;
    }

    size_t stride_len = decode_max_len(d);

    for (size_t i = 0; i < sample_len; i += stride_len) {
        size_t bufsize = stride_len;
        if ((sample_len - i) < bufsize) {
            bufsize = sample_len - i;
        }
        size_t symbol_len =
            demodulate(d->demod, samplebuf + i, bufsize, d->symbolbuf);

        if (d->opt.is_ofdm) {
            ofdmflexframesync_execute(d->frame.ofdm.framesync, d->symbolbuf,
                                      symbol_len);

            if (d->opt.is_debug) {
                char fname[50];
                sprintf(fname, "framesync_%u.out", d->i);
                ofdmflexframesync_debug_print(d->frame.ofdm.framesync, fname);
                d->i++;
            }
        } else {
            flexframesync_execute(d->frame.modem.framesync, d->symbolbuf,
                                  symbol_len);
            if (d->opt.is_debug) {
                char fname[50];
                sprintf(fname, "framesync_%u.out", d->i);
                flexframesync_debug_print(d->frame.modem.framesync, fname);
                d->i++;
            }
        }
    }

    return d->writebuf_accum;
}

size_t decode_flush(decoder *d) {
    if (!d) {
        return 0;
    }

    size_t symbol_len = demodulate_flush(d->demod, d->symbolbuf);

    if (d->opt.is_ofdm) {
        ofdmflexframesync_execute(d->frame.ofdm.framesync, d->symbolbuf,
                                  symbol_len);
    } else {
        flexframesync_execute(d->frame.modem.framesync, d->symbolbuf,
                              symbol_len);
    }

    return d->writebuf_accum;
}

void destroy_decoder(decoder *d) {
    if (!d) {
        return;
    }

    if (d->opt.is_ofdm) {
        ofdmflexframesync_destroy(d->frame.ofdm.framesync);
    } else {
        flexframesync_destroy(d->frame.modem.framesync);
    }
    destroy_demodulator(d->demod);
    free(d->symbolbuf);
    free(d->writebuf);
    free(d);
}